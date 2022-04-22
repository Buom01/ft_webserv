#include "webserv.hpp"

static Serve	*server		= NULL;
static bool	stop_requested	= false;
arguments_t	definedArgs;

void	stop_signal(int)
{
	if (server && server->alive())
		server->stop();
	stop_requested = true;
}

int main(int argc, char **argv)
{
	signal(SIGINT, stop_signal);
	std::ios::sync_with_stdio(false);

	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsMap		locations;
	Log						logger;

	std::vector<Redirect *>			redirectMiddlewares;
	std::vector<EjectBody *>		ejectBodyMiddlewares;
	std::vector<Upload *>			uploadMiddlewares;
	std::vector<Remover *>			removerMiddlewares;
	std::vector<CGI *>				cgiMiddlewares;
	std::vector<Static *>			staticMiddlewares;
	std::vector<Error *>			errorMiddlewares;
	std::vector<SendBodyFromFD *>	sendBodyFDMiddlewares;

	#pragma region Initiale check & Parse configuration file
	try
	{
		definedArgs = arguments(argc, argv);
		if (definedArgs.help == true)
		{
			Help	help;
			help.print();
			exit(EXIT_SUCCESS);
		}
		logger.options.verbose = definedArgs.verbose;
		config.init(definedArgs.configuration, true, true);
		config.check();
		servers = config.getServers();
		if (definedArgs.verbose == true)
			config.print();
	}
	catch (IncorrectArguments &e)
	{
		std::cerr << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	catch (std::ifstream::failure &e)
	{
		std::cerr << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	catch (Parse::IncorrectConfig &e)
	{
		std::cerr << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	if (stop_requested)
		return (EXIT_SUCCESS);
	#pragma endregion Initiale check & Parse configuration file

	#pragma region Start server

	server							= new Serve(logger);

	Error			*fallbackError	= new Error(server->logger);
	Mimetypes		*mimetypes		= new Mimetypes();
	SendBodyFromFD	*sendBodyFromFD	= new SendBodyFromFD(server->logger);

	mimetypes->useDefaults();

	server->use(parseStartLine, F_ALL);
	server->use(parseRequestHeaders, F_ALL);
	server->use(eject, F_ALL);
	server->use(body, F_NORMAL, M_ALL);
	
	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		ServerConfig			serverBlockConfig;
		Parse::mapListens		listen 				= config.listen((*it).options);
		Parse::stringVector		hostnames        	= config.serverName((*it).options);
		Parse::mapErrors		getErrors			= config.errorPage((*it).options);
		server_bind_t			*bound;

		serverBlockConfig.hostnames.insert(hostnames.begin(), hostnames.end());

		if (listen.size())
		{
			for (Parse::mapListens::const_iterator it = listen.begin(); it != listen.end(); it++)
			{
				bound	= server->bind((*it).ipSave, (*it).portSave, hostnames);

				if (bound)
					serverBlockConfig.interfaces.push_back(bound);
			}
		}
		else
		{
			bound	= server->bind("0.0.0.0", 80, hostnames);

			if (bound != NULL)
				serverBlockConfig.interfaces.push_back(bound);
			else
			{
				bound	= server->bind("0.0.0.0", 8000, hostnames);

				if (bound != NULL)
					serverBlockConfig.interfaces.push_back(bound);
			}
		}

		for (Parse::locationsMap::const_reverse_iterator itLoc = (*it).locations.rbegin(); itLoc != (*it).locations.rend(); itLoc++)
		{
			Parse::s_allow 						getAllow = config.allow((*itLoc).second);
			Parse::s_return						getReturn = config._return((*itLoc).second);
			Parse::s_clientBodyBufferSize		getBodyMaxSize = config.clientBodyBufferSize((*itLoc).second);
			std::string 						getRoot = config.root((*itLoc).second);
			std::string							getIndex = config.index((*itLoc).second);
			Parse::s_cgi						getCgi = config.cgi((*itLoc).second);
			std::pair<std::string, std::string>	getUpload = config.upload((*itLoc).second);
			method_t 							methods = method(getAllow);
			std::string							location_name = (*itLoc).first;

			if (getReturn.code != C_UNKNOWN)
			{
				Redirect	*redirect	= new Redirect(getReturn.code, getReturn.url);

				server->use(*redirect, F_NORMAL, methods, location_name, serverBlockConfig);
				redirectMiddlewares.push_back(redirect);
			}

			if  (getBodyMaxSize.isDefined)
            {
				EjectBody *ejectBody	= new EjectBody(getBodyMaxSize.size);

				server->use(*ejectBody, F_NORMAL, static_cast<method_t>(M_POST | M_PUT), location_name, serverBlockConfig);
				ejectBodyMiddlewares.push_back(ejectBody);
            }

			if (getCgi.isDefined)
			{
				CGI *_cgi	= new CGI(getCgi, location_name, getIndex);

				server->use(*_cgi, F_NORMAL, method(getCgi.allow), location_name, serverBlockConfig);
				cgiMiddlewares.push_back(_cgi);
			}
		}

		for (Parse::locationsMap::const_reverse_iterator itLoc = (*it).locations.rbegin(); itLoc != (*it).locations.rend(); itLoc++)
		{
			Parse::s_allow 						getAllow = config.allow((*itLoc).second);
			Parse::s_return						getReturn = config._return((*itLoc).second);
			Parse::s_autoindex					getAutoindex = config.autoindex((*itLoc).second);
			std::string 						getRoot = config.root((*itLoc).second);
			std::string							getIndex = config.index((*itLoc).second);
			Parse::s_cgi						getCgi = config.cgi((*itLoc).second);
			std::pair<std::string, std::string>	getUpload = config.upload((*itLoc).second);
			method_t 							methods = method(getAllow);
			std::string							location_name = (*itLoc).first;

			if (getAllow.isDefined)
				server->use(forbidden_method, F_ALL, static_cast<method_t>(~(methods)), location_name, serverBlockConfig);

			if (getUpload.first.length() && (methods & M_PUT))
			{
				Upload	*upload		= new Upload(server->logger, getUpload.first, getUpload.second);
				Remover	*remover	= new Remover(getUpload.first, getUpload.second);

				server->use(*upload, F_NORMAL, M_PUT, location_name, serverBlockConfig);
				server->use(*remover, F_NORMAL, M_DELETE, location_name, serverBlockConfig);
				uploadMiddlewares.push_back(upload);
				removerMiddlewares.push_back(remover);
			}

			if (getRoot.size())
			{
				Static	*serveStatic	= new Static();

				if (getAutoindex.isDefined)
					serveStatic->options.directory_listing	= getAutoindex.active;
				serveStatic->options.root				= getRoot;

				if (!getIndex.empty())
				{
					serveStatic->options.indexes.clear();
					serveStatic->options.indexes.push_back(getIndex);
				}

				server->use(*serveStatic, F_NORMAL, M_ALL, location_name, serverBlockConfig);
				staticMiddlewares.push_back(serveStatic);
			}

			if (getErrors.size())
			{
				Error			*error		= new Error(server->logger);

				error->options.errorpages = getErrors;

				server->use(*error, F_ALL, M_ALL, location_name, serverBlockConfig);
				errorMiddlewares.push_back(error);
			}
		}
	}

	server->use(*fallbackError, F_ALL, M_ALL);

	server->use(*mimetypes, F_ALL);
	server->use(addResponseHeaders, F_ALL);
	server->use(serializeHeaders, F_ALL);
	server->use(sendHeader, F_ALL);
	server->use(sendBodyFromBuffer, F_ALL);
	server->use(*sendBodyFromFD, F_ALL);

	server->use(awaitNextRequest, F_ALL);
	server->use(sendFinPacket, F_ALL);
	server->use(awaitClosed, F_ALL);

	errorMiddlewares.push_back(fallbackError);

	server->begin();
	#pragma endregion Start server

	#pragma region Run server
	while (server->alive())
	{
		server->accept();
		usleep(1);
	}
	#pragma endregion Run server


	#pragma region Middlewares cleanup 
	for (std::vector<Redirect *>::iterator it = redirectMiddlewares.begin(); it != redirectMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<EjectBody *>::iterator it = ejectBodyMiddlewares.begin(); it != ejectBodyMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<Upload *>::iterator it = uploadMiddlewares.begin(); it != uploadMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<Remover *>::iterator it = removerMiddlewares.begin(); it != removerMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<CGI *>::iterator it = cgiMiddlewares.begin(); it != cgiMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<Static *>::iterator it = staticMiddlewares.begin(); it != staticMiddlewares.end(); it++)
		delete (*it);

	for (std::vector<Error *>::iterator it = errorMiddlewares.begin(); it != errorMiddlewares.end(); it++)
		delete (*it);

	delete mimetypes;
	delete sendBodyFromFD;
	#pragma endregion Middlewares cleanup 

	delete server;
	return (EXIT_SUCCESS);
}
