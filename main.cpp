#include <iostream>
#include <unistd.h>
#include "Parse.hpp"
#include "Serve.hpp"
#include "Static.cpp"

int main(int argc, char **argv)
{
	Parse									config;
	Parse::serversVector					servers;
	Parse::locationsVector					locations;
	std::vector<std::pair<bool, Serve> >	serves;
	size_t									isAlive;

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
		Serve	server;
		Static	serverStatic;
	
		Parse::s_autoindex autoindex = config.autoindex((*it).options);
		Parse::s_listen bind = config.listen((*it).options);

		serverStatic.options.root = config.root((*it).options);
		serverStatic.options.directory_listing = autoindex.active;
		serverStatic.options.indexes.push_back(config.index((*it).options));
		server.bind(bind.ipSave, bind.port);
		
		#pragma region Add middleware here
		
		app.use(parseStartLine, F_ALL);
		app.use(parseRequestHeaders, F_ALL);
		app.use(cgi);
		app.use(serveStatic);
		app.use(error, F_ALL);
		app.use(addResponseHeaders, F_ALL);
		app.use(serializeHeaders, F_ALL);
		app.use(sendHeader, F_ALL);
		app.use(sendBodyFromBuffer, F_ALL);
		app.use(sendBodyFromFD, F_ALL);
		
		#pragma endregion Add middleware here

		server.begin();
		serves.push_back(std::make_pair(true, server));
	}

	isAlive = serves.size();
	while (isAlive > 0)
	{
		for (std::vector<std::pair<bool, Serve> >::const_iterator it = serves.begin(); it != serves.end(); it++)
		{
			if ((*it)->first == true)
			{
				if (!(*it)->second.alive())
				{
					(*it)->first = false;
					--isAlive;
				}
				else
					(*it).second.accept();
			}
		}
		usleep(10);
	}
	#pragma endregion Start server
	return (EXIT_SUCCESS);
}
