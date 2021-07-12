#include <iostream>
#include "Parse.hpp"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return (1);
	try { Parse file(argv[1]); }
	catch (std::ifstream::failure &e) { std::cerr << e.what() << std::endl; }
	catch (Parse::IncorrectConfig &e) { std::cerr << e.what() << std::endl; }

	return (0);
}
