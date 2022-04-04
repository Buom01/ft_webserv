#include <iostream>
#include <exception>
#include <unistd.h>
#include "Parse.hpp"
#include "Serve.hpp"
#include "Static.cpp"
#include "read.cpp"
#include "Cgi.hpp"
#include "Response.hpp"
#include "write_headers.cpp"
#include "write_body.cpp"

int main(int argc, char **argv)
{
	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsVector	locations;
	std::vector<Serve>		serves;

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
	#pragma endregion Parse configuration file
	

	#pragma region Start server
	for (Parse::serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		Serve			server;
		Static			serverStatic;
		SendBodyFromFD	sendBodyFromFD(server.logger);
		Error			error(server.logger);
	
		Parse::s_autoindex autoindex = config.autoindex((*it).options);
		Parse::s_listen bind = config.listen((*it).options);

		serverStatic.options.root = config.root((*it).options);
		serverStatic.options.directory_listing = autoindex.active;
		serverStatic.options.indexes.push_back(config.index((*it).options));
		//error.options.errorpages = config.
		server.bind(bind.ipSave, bind.port);
		
		#pragma region Add middleware here
		server.use(parseStartLine, F_ALL);
		server.use(parseRequestHeaders, F_ALL);
		server.use(cgi);
		server.use(serverStatic);
		//server.use(error, F_ALL);
		server.use(addResponseHeaders, F_ALL);
		server.use(serializeHeaders, F_ALL);
		server.use(sendHeader, F_ALL);
		server.use(sendBodyFromBuffer, F_ALL);
		server.use(sendBodyFromFD, F_ALL);
		#pragma endregion Add middleware here
		
		server.begin();
		serves.push_back(server);
	}

	while (serves.size() > 0)
	{
		for (std::vector<Serve>::iterator it = serves.begin(); it != serves.end(); it++)
		{
			if ((*it).alive() == false)
				serves.erase(it);
			else
				(*it).accept();
			usleep(10);
		}
	}
	#pragma endregion Start server

	return (EXIT_SUCCESS);
}
