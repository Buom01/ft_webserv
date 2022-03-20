#include <iostream>
#include "Parse.hpp"
#include "Serve.hpp"
#include "Static.cpp"

int main(int argc, char **argv)
{
	Parse					config;
	Parse::serversVector	servers;
	Parse::locationsVector	locations;

	std::vector<Serve>		serves;
	
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

	#pragma region Parse configuration file
	try
	{
		config.init(argv[1]);
		config.check();
		servers = config.getServers();
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
		Serve newServer;
		Parse::s_listen bind = config.listen((*it).options);
		newServer.bind(bind.ipSave, bind.portSave);
	}
	#pragma endregion Start server
	
	return (EXIT_SUCCESS);
}
