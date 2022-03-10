#include <iostream>
#include "Parse.hpp"

//#include "Cgi.hpp"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return (1);
	try
	{
		Parse config(argv[1]);
		Parse::optionsVec generalOptions = config.getOptions();

		std::string test = config.alias(generalOptions);
		Parse::s_allow allow = config.allow(generalOptions);
		std::cout << "DELETE=" << allow.DELETE << std::endl;
		std::cout << "GET=" << allow.GET << std::endl;
		std::cout << "POST=" << allow.POST << std::endl;
		std::cout << "PUT=" << allow.PUT << std::endl;

		//config.print();
		//CGI cgi(config);
		//cgi.printCGI();
	}
	catch (std::ifstream::failure &e) { std::cerr << e.what() << std::endl; }
	//catch (Parse::IncorrectConfig &e) { std::cerr << e.what() << std::endl; }
	return (0);
}
