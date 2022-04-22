#ifndef __LIB_ARGV_HPP
# define __LIB_ARGV_HPP
# define DEFAULT_CONF "default.conf"
# include <iostream>
# include <sstream>
# include <string>
# include <fstream>
# include <algorithm>
# include "define.hpp"
# include "file.hpp"

struct	arguments_t
{
	bool		help;
	bool		verbose;
	std::string	configuration;

	arguments_t();
	arguments_t(bool, bool, std::string);
};
class	IncorrectArguments : virtual public std::exception
{
	private:
		const std::string e;
	public:
		IncorrectArguments();
		IncorrectArguments(const IncorrectArguments &copy);
		IncorrectArguments &operator=(const IncorrectArguments &newObject);
		IncorrectArguments(const std::string &_e) : e(_e) {};
		~IncorrectArguments() throw() {};
		const char* what() const throw() { return e.c_str(); }
};

std::string argumentsError(std::string);
bool		checkDefaultConfig();
arguments_t	arguments(int, char **);

#endif
