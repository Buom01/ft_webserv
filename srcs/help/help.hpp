#ifndef __HELP
# define __HELP
# include <cstdio>
# include <iostream>
# include <fstream>
# include <string>
# include "File.hpp"
# include "Regex.hpp"

void help()
{
	Regex 			regex;
	std::ifstream	stream;
	std::string 	line("");
	size_t 			pos(0);

	stream.open("srcs/help/documentation");
	stream.exceptions(std::ifstream::badbit);
	if (!stream.is_open())
		throw std::ifstream::failure("Open help file failed");
	while (getline(stream, line))
	{
		regex.exec(line, "\\\\e\\[([0-9]+)m", GLOBAL_FLAG);
		if (regex.size())
		{
			pos = 0;
			for (size_t x = 0; x < regex.size(); x++)
			{
				std::cout << line.substr(pos, regex.match()[x].group.start - pos);
				std::cout << "\e[" << std::atoi(regex.match()[x].occurence.c_str()) << "m";
				pos = regex.match()[x].group.end;
				if (pos >= line.size())
					break;
			}
			if (pos < line.size())
				std::cout << line.substr(pos);
			std::cout << std::endl;
		}
		else
			std::cout << line << std::endl;
	}
	stream.close();
}

#endif
