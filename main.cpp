#include <iostream>
#include <exception>
#include <unistd.h>
#include "Parse.hpp"
#include "Serve.hpp"
#include "utils.hpp"
#include "Static.cpp"
#include "error.cpp"
#include "read.cpp"
#include "eject.cpp"
#include "upload.cpp"
#include "remover.cpp"
#include "Cgi.hpp"
#include "Response.hpp"
//#include "readToTrashbin.cpp"
#include "write_headers.cpp"
#include "write_body.cpp"
#include "forbidden.cpp"
#include "mimetypes.cpp"


Serve	*server;

void	stop_signal(int)
{
	if (server->alive())
		server->stop();
}

int main(int argc, char **argv)
{
	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsMap		locations;

	std::vector<Eject *>			ejectMiddlewares;
	std::vector<Upload *>			uploadMiddlewares;
	std::vector<Remover *>			removerMiddlewares;
	std::vector<CGI *>				cgiMiddlewares;
	std::vector<Static *>			staticMiddlewares;
	std::vector<Error *>			errorMiddlewares;
	std::vector<SendBodyFromFD *>	sendBodyFDMiddlewares;

	#pragma region Initiale check & Parse configuration file
	if (argc <= 1)
	{
		std::cerr << "WEBSERV - No configuration file has been passed" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (argc >= 3)
	{
		std::cerr << "WEBSERV - Only one argument is allowed" << std::endl;
		return (EXIT_FAILURE);
	}
	try
	{
		config.init(argv[1]);
		config.check();
		servers = config.getServers();
		config.print();
	}
	catch (std::ifstream::failure &e)
	{
		std::cerr << "WEBSERV - " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	catch (Parse::IncorrectConfig &e)
	{
		std::cerr << "WEBSERV - " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	#pragma endregion Initiale check & Parse configuration file
	
	#pragma region Start server

	signal(SIGINT, stop_signal);

	server							= new Serve();

	Eject			*preEject		= new Eject(-1);
	Error			*fallbackError	= new Error(server->logger);
	Mimetypes		*mimetypes		= new Mimetypes();
	SendBodyFromFD	*sendBodyFromFD	= new SendBodyFromFD(server->logger);

	mimetypes->useDefaults();

	server->use(parseStartLine, F_ALL);
	server->use(parseRequestHeaders, F_ALL);
	server->use(*preEject, F_ALL);
	
	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		ServerConfig		serverBlockConfig;
		Parse::mapListens	listen 				= config.listen((*it).options);
		Parse::stringVector	hostnames        	= config.serverName((*it).options);

		serverBlockConfig.hostnames = hostnames;

		if (listen.size())
		{
			int	bound;
			
			for (Parse::mapListens::const_iterator it = listen.begin(); it != listen.end(); it++)
			{
				bound	= server->bind((*it).ipSave, (*it).portSave);

				if (bound > 0)
					serverBlockConfig.interfaces.push_back(bound);
			}
		}
		else
		{
			int	bound;
			
			bound	= server->bind("0.0.0.0", 80);
			if (bound > 0)
				serverBlockConfig.interfaces.push_back(bound);
			else
			{
				bound	= server->bind("0.0.0.0", 8000);
				if (bound > 0)
					serverBlockConfig.interfaces.push_back(bound);
			}
		}

		for (Parse::locationsMap::const_reverse_iterator itLoc = (*it).locations.rbegin(); itLoc != (*it).locations.rend(); itLoc++)
		{
			Parse::s_allow 						getAllow = config.allow((*itLoc).second);
			Parse::s_clientBodyBufferSize		getBodyMaxSize = config.clientBodyBufferSize((*itLoc).second);
			Parse::s_autoindex					getAutoindex = config.autoindex((*itLoc).second);
			std::string 						getRoot = config.root((*itLoc).second, true);
			std::string							getIndex = config.index((*itLoc).second);
			Parse::mapErrors 					getErrors = config.errorPage((*itLoc).second);
			Parse::s_cgi						getCgi = config.cgi((*itLoc).second);
			std::pair<std::string, std::string>	getUpload = config.upload((*itLoc).second);
			method_t 							methods = method(getAllow);

			std::string	location_name = (*itLoc).first;

			if (getAllow.isDefined)
				server->use(forbidden_method, F_ALL, static_cast<method_t>(~(methods)), location_name, serverBlockConfig);

			if (getBodyMaxSize.isDefined)
			{
				Eject	*eject	= new Eject(getBodyMaxSize.size);

				server->use(*eject, F_NORMAL, M_ALL, location_name, serverBlockConfig);
				ejectMiddlewares.push_back(eject);
			}

			if (getUpload.first.length() && (methods & M_PUT))
			{
				Upload	*upload		= new Upload(server->logger, getUpload.first, getUpload.second);
				Remover	*remover	= new Remover(getUpload.first, getUpload.second);

				server->use(*upload, F_NORMAL, M_PUT, location_name, serverBlockConfig);
				server->use(*remover, F_NORMAL, M_DELETE, location_name, serverBlockConfig);
				uploadMiddlewares.push_back(upload);
				removerMiddlewares.push_back(remover);
			}

			if (getCgi.isDefined)
			{
				CGI *_cgi = new CGI(getCgi, getIndex);
				server->use(*_cgi, F_NORMAL, method(getCgi.allow), location_name, serverBlockConfig);
				cgiMiddlewares.push_back(_cgi);
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
	// server->use(readToTrashbin, F_ALL);
	server->use(sendHeader, F_ALL);
	server->use(sendBodyFromBuffer, F_ALL);
	server->use(*sendBodyFromFD, F_ALL);

	ejectMiddlewares.push_back(preEject);
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

	/*for (std::vector<Body *>::iterator it = bodyMiddlewares.begin(); it != bodyMiddlewares.end(); it++)
	{
		delete (*it);
	}*/

	for (std::vector<Eject *>::iterator it = ejectMiddlewares.begin(); it != ejectMiddlewares.end(); it++)
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
	
	return (EXIT_SUCCESS);
}
