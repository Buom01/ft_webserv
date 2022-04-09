#include <iostream>
#include <exception>
#include <unistd.h>
#include "Parse.hpp"
#include "Serve.hpp"
#include "Static.cpp"
#include "error.cpp"
#include "read.cpp"
#include "eject.cpp"
#include "Cgi.hpp"
#include "Response.hpp"
#include "write_headers.cpp"
#include "write_body.cpp"
#include "forbidden.cpp"
#include "mimetypes.cpp"

std::vector<Serve *>	serversApp;

void	stop_signal(int)
{
	for (std::vector<Serve *>::iterator it = serversApp.begin(); it != serversApp.end(); it++)
	{
		if ((*it)->alive() == true)
			(*it)->stop();
	}
}

method_t method(Parse::s_allow allow)
{
	method_t ret = M_UNKNOWN;
	if (allow.GET)
		ret = static_cast<method_t>(ret | M_GET);
	if (allow.POST)
		ret = static_cast<method_t>(ret | M_POST);
	if (allow.PUT)
		ret = static_cast<method_t>(ret | M_PUT);
	if (allow.DELETE)
		ret = static_cast<method_t>(ret | M_DELETE);
	return ret;
}

int main(int argc, char **argv)
{
	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsMap		locations;

	std::vector<Eject *>			ejectMiddlewares;
	std::vector<Static *>			staticMiddlewares;
	std::vector<Error *>			errorMiddlewares;
	std::vector<Mimetypes *>		mimetypesMiddlewares;
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

	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		Serve			*server			= new Serve();
		Eject			*preEject		= new Eject(-1);
		Error			*fallbackError	= new Error(server->logger);
		Mimetypes		*mimetypes		= new Mimetypes();
		SendBodyFromFD	*sendBodyFromFD	= new SendBodyFromFD(server->logger);

		mimetypes->add("html", "text/html");
	
		Parse::s_listen bind 					= config.listen((*it).options);
		std::vector<std::string> server_name	= config.serverName((*it).options);
		
		server->bind(bind.ipSave, bind.portSave);

		server->use(parseStartLine, F_ALL);
		server->use(parseRequestHeaders, F_ALL);
		server->use(*preEject, F_ALL);

		for (Parse::locationsMap::const_iterator itLoc = (*it).locations.begin(); itLoc != (*it).locations.end(); itLoc++)
		{
			Parse::s_allow 					getAllow = config.allow((*itLoc).second);
			Parse::s_clientBodyBufferSize	getBodyMaxSize = config.clientBodyBufferSize((*itLoc).second);
			Parse::s_autoindex				getAutoindex = config.autoindex((*itLoc).second);
			std::string 					getRoot = config.root((*itLoc).second, true);
			std::string						getIndex = config.index((*itLoc).second);
			Parse::mapErrors 				getErrors = config.errorPage((*itLoc).second);
			Parse::s_cgi					getCgi = config.cgi((*itLoc).second);
			method_t 						methods = method(getAllow);

			std::string	location_name = (*itLoc).first;

			if (getAllow.isDefined)
				server->use(forbidden, F_ALL, static_cast<method_t>(~(methods)), location_name);  // Should only be used on the lattest location block middleware

			if (getBodyMaxSize.isDefined)  // Conditional jump or move depends on uninitialised value(s)
			{
				Eject	*eject	= new Eject(getBodyMaxSize.size);

				server->use(*eject, F_NORMAL, M_ALL, location_name);
				ejectMiddlewares.push_back(eject);
			}

			if (getCgi.isDefined)
				server->use(cgi, F_NORMAL, M_ALL, location_name);

			if (getRoot.size())
			{
				Static	*serveStatic	= new Static();

				serveStatic->options.directory_listing	= getAutoindex.active;
				serveStatic->options.root				= getRoot;

				if (!getIndex.empty())
				{
					serveStatic->options.indexes.clear();
					serveStatic->options.indexes.push_back(getIndex);
				}

				server->use(*serveStatic, F_NORMAL, M_ALL, location_name);
				staticMiddlewares.push_back(serveStatic);
			}

			if (getErrors.size())
			{
				Error			*error		= new Error(server->logger);

				error->options.errorpages = getErrors;

				server->use(*error, F_ALL, M_ALL, location_name);
				errorMiddlewares.push_back(error);
			}
		}
		server->use(*fallbackError, F_ALL, M_ALL);

		server->use(*mimetypes, F_ALL);
		server->use(addResponseHeaders, F_ALL);
		server->use(serializeHeaders, F_ALL);
		server->use(sendHeader, F_ALL);
		server->use(sendBodyFromBuffer, F_ALL);
		server->use(*sendBodyFromFD, F_ALL);
		server->begin();
		serversApp.push_back(server);
		ejectMiddlewares.push_back(preEject);
		errorMiddlewares.push_back(fallbackError);
		mimetypesMiddlewares.push_back(mimetypes);
		sendBodyFDMiddlewares.push_back(sendBodyFromFD);
	}
	#pragma endregion Start server


	#pragma region Run server
	std::vector<Serve *>::iterator appIt;
	while (serversApp.size() > 0)
	{
		appIt = serversApp.begin();
		
		while (appIt != serversApp.end())
		{
			if ((*appIt)->alive() == false)
			{
				delete (*appIt);
				appIt = serversApp.erase(appIt);
			}
			else
			{
				(*appIt)->accept();
				++appIt;
			}
		}

		usleep(1);
	}
	#pragma endregion Run server

	#pragma region Middlewares cleanup 
	for (std::vector<Eject *>::iterator it = ejectMiddlewares.begin(); it != ejectMiddlewares.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<Static *>::iterator it = staticMiddlewares.begin(); it != staticMiddlewares.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<Error *>::iterator it = errorMiddlewares.begin(); it != errorMiddlewares.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<Mimetypes *>::iterator it = mimetypesMiddlewares.begin(); it != mimetypesMiddlewares.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<SendBodyFromFD *>::iterator it = sendBodyFDMiddlewares.begin(); it != sendBodyFDMiddlewares.end(); it++)
	{
		delete (*it);
	}
	#pragma endregion Middlewares cleanup 
	
	return (EXIT_SUCCESS);
}
