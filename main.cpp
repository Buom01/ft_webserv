#include <iostream>
#include "Regex.hpp"
//#include "Parse.hpp"

//#include "Cgi.hpp"

int main(int argc, char **argv)
{
	(void)argv;
	(void)argc;
	//if (argc <= 1)
	//	return (1);
	try
	{
		Regex regex;

		regex.exec("GGET de DE  DEDEDEDED  DMFKORK KEE", "([a-zA-Z0-9]+)");
		//regex.print();

		regex.exec("server_name		example.net www.example.net;", "^[ \t]*server_name[ \t]+([-a-zA-Z0-9. \t]*);$", GLOBAL_FLAG);
		//regex.print();
		
		regex.exec("cgi 			.php .perl /usr/local/bin/php-cgi GET POST;", "^[ \t]*cgi[ \t]+([a-zA-Z0-9_. \t]*)(\\/[-a-zA-Z0-9_\\/._]*)[ \t]*(.*);$", GLOBAL_FLAG);
		regex.print();

		/*Parse config(argv[1]);
		Parse::optionsVec generalOptions = config.getOptions();

		std::string test = config.alias(generalOptions);
		Parse::s_allow allow = config.allow(generalOptions);
		std::cout << "DELETE=" << allow.DELETE << std::endl;
		std::cout << "GET=" << allow.GET << std::endl;
		std::cout << "POST=" << allow.POST << std::endl;
		std::cout << "PUT=" << allow.PUT << std::endl;*/

		//config.print();
		//CGI cgi(config);
		//cgi.printCGI();
	}
	catch (std::ifstream::failure &e) { std::cerr << e.what() << std::endl; }
	//catch (Parse::IncorrectConfig &e) { std::cerr << e.what() << std::endl; }
	return (0);
}
