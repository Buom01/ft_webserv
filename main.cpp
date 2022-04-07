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

int main(int argc, char **argv)
{
	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsMap		locations;
	std::vector<Serve *>	serves;

	std::vector<Eject *>			ejectMiddlewares;
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
	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		Serve			*server			= new Serve();
		Eject			*eject			= new Eject();
		Static			*serveStatic	= new Static();
		Error			*error			= new Error(server->logger);
		SendBodyFromFD	*sendBodyFromFD	= new SendBodyFromFD(server->logger);
	
		Parse::s_autoindex	autoindex	= config.autoindex((*it).options);
		Parse::s_listen		bind		= config.listen((*it).options);

		serveStatic->options.root				= config.root((*it).options);
		serveStatic->options.directory_listing	= autoindex.active;
		serveStatic->options.indexes.push_back(config.index((*it).options));
		//error->options.errorpages				= config.
		server->bind(bind.ipSave, bind.port);
		
		#pragma region Add middleware here
		server->use(parseStartLine, F_ALL);
		server->use(parseRequestHeaders, F_ALL);
		server->use(*eject);

		server->use(cgi);
		server->use(*serveStatic);
		server->use(*error, F_ALL);

		server->use(addResponseHeaders, F_ALL);
		server->use(serializeHeaders, F_ALL);
		server->use(sendHeader, F_ALL);
		server->use(sendBodyFromBuffer, F_ALL);
		server->use(*sendBodyFromFD, F_ALL);
		#pragma endregion Add middleware here
		
		server->begin();
		serves.push_back(server);
		ejectMiddlewares.push_back(eject);
		staticMiddlewares.push_back(serveStatic);
		errorMiddlewares.push_back(error);
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
