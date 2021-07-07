#include <iostream>
#include "parse.hpp"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return (1);
	Parse file(argv[1]);

	return (0);
}
