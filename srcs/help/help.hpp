#ifndef __HELP
# define __HELP
# include <iostream>
# include <fstream>
# include <string>
# include "Regex.hpp"
# include "generateDocumentation.hpp"

class Help
{
	private:
		Regex 			regex;
		size_t 			pos;
	public:
		Help(): pos(0) {};
		~Help() {};
	private:
		void	parseData(std::string line)
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

		void	dev()
		{
			std::fstream	stream;
			std::string line("");

			stream.open("./srcs/help/documentation");
			stream.exceptions(std::ifstream::badbit);
			if (!stream.is_open())
				throw std::ifstream::failure("Open help file failed");
			while (getline(stream, line))
				parseData(line);
			stream.close();
		}

		void	prod()
		{
			std::stringstream ss;
			std::string line(""), buf("");
			
			buf.append(reinterpret_cast<char *>(&__srcs_help_documentation));
			ss.str(buf);
			while (getline(ss, line))
				parseData(line);
		}
	public:
		void print(bool isDev = false)
		{
			if (isDev)
				dev();
			else
				prod();
		}
};

#endif
