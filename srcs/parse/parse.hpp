/**
 * 	Parsing config file
 */
# include <iostream>
# include <fstream>
# include <cstring>
# include <exception>
# include "regex.hpp"

struct line
{
	bool		IsBlock;
	std::string Key;
	std::string	Value;
};

class Parse
{
	private:
		const char		*_configPath;
		std::string		_line;
		std::ifstream	CONFIG;
		Regex			regex;
	public:
		Parse(const char *config_path) : _configPath(config_path)
		{
			CONFIG.open(_configPath);
			CONFIG.exceptions(std::ifstream::badbit);
			if (!CONFIG.is_open())
				throw std::ifstream::failure("Open configuration file failed");
			#pragma region File struct with data
			
			while (getline(CONFIG, _line))
			{
				regex.Match(_line, "^.\\w*\\b");
				for (size_t x = 0; x < regex.GetSize(); x++)
				{
					std::cout << "-" << regex.GetMatch()[x].occurence << "-" << regex.GetMatch()[x].start << "-" << regex.GetMatch()[x].end << std::endl;
				}
			}
			CONFIG.close();
			#pragma endregion
		}
		virtual ~Parse() {}
	private:
		Parse(void) {}
};
