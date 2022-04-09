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
	std::vector<Serve *>	serves;

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
	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		Serve			*server			= new Serve();
		Eject			*eject			= new Eject();
		Static			*serveStatic	= new Static();
		Error			*error			= new Error(server->logger);
		Mimetypes		*mimetypes		= new Mimetypes();
		SendBodyFromFD	*sendBodyFromFD	= new SendBodyFromFD(server->logger);

		mimetypes->add("html", "text/html");
	
		Parse::s_listen bind 					= config.listen((*it).options);
		// Parse::s_clientBodyBufferSize bodySize 	= config.clientBodyBufferSize((*it).options);
		std::vector<std::string> server_name	= config.serverName((*it).options);
		
		server->bind(bind.ipSave, bind.portSave);
		// if (bodySize.isDefined)  // Conditional jump or move depends on uninitialised value(s)
		// 	eject->max_payload_size = bodySize.size;

		server->use(parseStartLine, F_ALL);
		server->use(parseRequestHeaders, F_ALL);
		server->use(*eject, F_ALL);

		if (!((*it).locations.empty()))
		{
			for (Parse::locationsMap::const_iterator itLoc = (*it).locations.begin(); itLoc != (*it).locations.end(); itLoc++)
			{
				Parse::s_allow 					getAllow = config.allow((*itLoc).second);
				// Parse::s_clientBodyBufferSize	getBodySize = config.clientBodyBufferSize((*itLoc).second);
				Parse::s_autoindex				getAutoindex = config.autoindex((*itLoc).second);
				std::string 					getRoot = config.root((*itLoc).second, true);
				std::string						getIndex = config.index((*itLoc).second);
				Parse::mapErrors 				getErrors = config.errorPage((*itLoc).second);
				Parse::s_cgi					getCgi = config.cgi((*itLoc).second);
				method_t 						methods = method(getAllow);

				serveStatic->options.directory_listing	= getAutoindex.active;
				serveStatic->options.root				= getRoot;

				if (!getIndex.empty())
				{
					serveStatic->options.indexes.clear();
					serveStatic->options.indexes.push_back(getIndex);
				}

				// if (getBodySize.isDefined) // Conditional jump or move depends on uninitialised value(s)
				// 	eject->max_payload_size = getBodySize.size;
				
				for (Parse::mapErrors::const_iterator itErr = getErrors.begin(); itErr != getErrors.end(); itErr++)
					error->add((*itErr).first, (*itErr).second);

				server->use(forbidden, F_ALL, static_cast<method_t>(~(methods)), (*itLoc).first);  // SHould only be used on the lattest location block middleware
				if (getCgi.isDefined)
					server->use(cgi, F_ALL, M_ALL, (*itLoc).first);
				server->use(*serveStatic, F_ALL, M_ALL, (*itLoc).first);
				server->use(*error, F_ALL, M_ALL, (*itLoc).first);
			}
		}

		server->use(*mimetypes, F_ALL);
		server->use(addResponseHeaders, F_ALL);
		server->use(serializeHeaders, F_ALL);
		server->use(sendHeader, F_ALL);
		server->use(sendBodyFromBuffer, F_ALL);
		server->use(*sendBodyFromFD, F_ALL);
		server->begin();
		serves.push_back(server);
		ejectMiddlewares.push_back(eject);
		staticMiddlewares.push_back(serveStatic);
		errorMiddlewares.push_back(error);
		mimetypesMiddlewares.push_back(mimetypes);
		sendBodyFDMiddlewares.push_back(sendBodyFromFD);
	}

	while (serves.size() > 0)
	{
		for (std::vector<Serve *>::iterator it = serves.begin(); it != serves.end(); it++)
		{
			if ((*it)->alive() == false)
				serves.erase(it);
			else
				(*it)->accept();
			usleep(10);
		}
	}
	#pragma endregion Start server

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

	for (std::vector<SendBodyFromFD *>::iterator it = sendBodyFDMiddlewares.begin(); it != sendBodyFDMiddlewares.end(); it++)
	{
		delete (*it);
	}
	#pragma endregion Middlewares cleanup 
	
	return (EXIT_SUCCESS);
}
