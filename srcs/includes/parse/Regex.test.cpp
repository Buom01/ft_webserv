#include "Regex.hpp"

int	main(int argc, char **argv)
{
	std::string line, reg;
	Regex	regex;

	if (argc < 3)
	{
		std::cout << "No args, default value set" << std::endl;
		line = "One     Two Three Four Five Six ";
		reg = "([a-zA-Z0-9_]+)";
	}
	else
	{
		line = argv[0];
		reg = argv[1];
	}
	std::cout << "Testing: \"" << line << "\"" << std::endl;
	std::cout << "With: \"" << reg << "\"" << std::endl;

	std::cout << "With no flag:" << std::endl;
	while (regex.exec(line, reg))
	{
		if (regex.size() <= 0)
			break;
		regex.print();
	}
	std::cout << "Restart string to start occurence with reset to true" << std::endl;
	regex.exec(line, reg, NO_FLAG, true);
	regex.print();
	regex.exec(line, reg, NO_FLAG, true);
	regex.print();
	regex.exec(line, reg, NO_FLAG);
	regex.print();

	std::cout << "With GLOBAL_FLAG:" << std::endl;
	regex.exec(line, reg, GLOBAL_FLAG, true);
	regex.print();
	return 0;
}
