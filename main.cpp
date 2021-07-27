#include <iostream>
#include "Parse.hpp"
#include "Cgi.hpp"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return (1);
	try
	{
		Parse config(argv[1]);
		CGI cgi(config);
		cgi.printCGI();
	}
	catch (std::ifstream::failure &e) { std::cerr << e.what() << std::endl; }
	catch (Parse::IncorrectConfig &e) { std::cerr << e.what() << std::endl; }
	return (0);
}
