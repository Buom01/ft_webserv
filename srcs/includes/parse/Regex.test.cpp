#include "Regex.hpp"

int	main(int argc, char **argv)
{
	Regex	regex;

	if (argc < 3)
	{
		std::cout << "Regex tester" << std::endl;
		std::cout << "Usage: [TEXT] [REGEX]" << std::endl;
		return 1;
	}
	argv++;
	std::cout << "Testing: \"" << argv[0] << "\"" << std::endl;
	std::cout << "With: \"" << argv[1] << "\"" << std::endl;
	
	std::cout << "With no flag:" << std::endl;
	regex.exec(std::string(argv[0]), std::string(argv[1]));
	regex.print();

	std::cout << "With GLOBAL_FLAG:" << std::endl;
	regex.exec(std::string(argv[0]), std::string(argv[1]), GLOBAL_FLAG);
	regex.print();
	return 0;
}
