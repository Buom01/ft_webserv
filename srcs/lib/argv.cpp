#include "argv.hpp"

arguments_t::arguments_t():
	help(false),
	verbose(false),
	configuration("")
{}

arguments_t::arguments_t(bool _help, bool _verbose, std::string _config):
	help(_help),
	verbose(_verbose),
	configuration(_config)
{}

std::string argumentsError(std::string line)
{
	std::string err(COLOR_ERROR);
	err.append(line);
	err.append(COLOR_RESET);
	throw IncorrectArguments(err);
}

bool checkDefaultConfig()
{
	std::ifstream	file;
		
	file.open(DEFAULT_CONF);
	file.exceptions(std::ifstream::badbit);
	if (!file.is_open())
	{
		std::cout << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - no default configuration file is present, webserv now stop" << std::endl;
		return false;
	}
	std::cout << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - a default configuration is present, webserv now start" << std::endl;
	return true;
}

arguments_t	arguments(int argc, char **argv)
{
	bool		isConfig(false);
	std::string temp("");
	arguments_t ret(false, false, "");

	if (argc < 2)
	{
		std::cout << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - no arguments, check if default configuration exist at the root folder" << std::endl;
		if (!checkDefaultConfig())
			exit(EXIT_FAILURE);
		ret.configuration = DEFAULT_CONF;
		return ret;
	}
	for (int x = 1; x < argc; x++)
	{
		temp = argv[x];
		if (temp.at(0) == '-')
		{
			if (isConfig)
				argumentsError("no option is allowed after the definition of the configuration file");
			if (temp.compare("--help") == 0 || temp.compare("-h") == 0)
			{
				if (ret.help == true)
					argumentsError("help option is specified several times");
				ret.help = true;
				break;
			}
			else if (temp.compare("--verbose") == 0 || temp.compare("-v") == 0)
			{
				if (ret.verbose == true)
					argumentsError("verbose option is specified several times");
				ret.verbose = true;
			}
			else
			{
				std::string err("option ");
				err.append(COLOR_WARNING);
				err.append(temp);
				err.append(COLOR_ERROR);
				err.append(" is not recognized");
				argumentsError(err);
			}
		}
		else
		{
			if (isConfig)
				argumentsError("no arguments are allowed after the definition of the configuration file");
			ret.configuration = temp;
			if (getExtension(temp) != "conf")
				argumentsError("the configuration file does not have the .conf extension");
			isConfig = true;
		}
	}
	if (!isConfig && !ret.help)
	{
		std::cout << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - no configuration, check if default configuration exist at the root folder" << std::endl;
		if (!checkDefaultConfig())
			exit(EXIT_FAILURE);
	}
	return (ret);
}
