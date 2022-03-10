#ifndef __PARSE
# define __PARSE
# define REGEX_SIZE 13
# include <iostream>
# include <fstream>
# include <cstring>
# include <cstdarg>
# include <exception>
# include <map>
# include <vector>
# include <string>
# include <arpa/inet.h>
# include "Regex.hpp"
# include "nullptr_t.hpp"

struct s_defineRegex
{
	std::string		name;
	std::string		regex;
	bool			noDuplication;
} REGEX[REGEX_SIZE] =
{
	{ "location", "^[ \t]*location[ \t]+([a-zA-Z0-9_/.]*)[ \t]*\\{*$", false},
	{ "endLocation", "^[ \t]*(});*$", false },
	{ "alias", "^[ \t]*alias[ \t]+([-a-zA-Z0-9_.\\/]+);$", true },
	{ "allow", "^[ \t]*allow[ \t]+(.*);$", true },
	{ "autoindex", "^[ \t]*autoindex[ \t]+(on|off);$", true },
	{ "cgi", "^[ \t]*cgi[ \t]+([a-zA-Z0-9_. \t]*)(\\/[-a-zA-Z0-9_\\/._]*)[ \t]*(.*);$", true },
	{ "client_body_buffer_size", "^[ \t]*client_body_buffer_size[ \t]+([0-9]*)(b|k|m|g);$", true },
	{ "error_page", "^[ \t]*error_page[ \t]+([0-9 ]*)(\\/.*);$", false },
	{ "index", "^[ \t]*index[ \t]+(.*);$", true },
	{ "root", "^[ \t]*root[ \t]+(\\/.*);$", true },
	{ "server_name", "^[ \t]*server_name[ \t]+([-a-zA-Z0-9. \t]*);$", true },
	{ "listen", "^[ \t]*listen[ \t]+(.+);$", true }
};

struct ParseTypedef
{
	typedef std::vector<std::string>				defaultVector;
	typedef std::pair<std::string, defaultVector>	stringPair;
	typedef std::vector<stringPair>					optionsVec;
	typedef optionsVec::iterator					optionsIt;
	
	typedef std::vector<optionsVec>					locationsVec;
	typedef locationsVec::iterator					locationsIt;

	/**
	 * @param GET (bool), false by default
	 * @param PUT (bool), false by default
	 * @param POST (bool), false by default
	 * @param DELETE (bool), false by default
	 */
	struct s_allow
	{
		bool GET, PUT, POST, DELETE;
	};

	/**
	 * @param active (bool), false by default
	 */
	struct s_autoindex
	{
		bool active;
	};

	/**
	 * @param extensions (std::vector<<std::string>>), file extensions
	 * @param path (std::string), path of executable
	 * @param methods (s_allow), accepted HTTP request methods
	 */
	struct s_cgi
	{
		std::vector<std::string>	extensions;
		std::string					path;
		s_allow						methods;							
	};

	/**
	 * @param (int) bit size
	 * @param (int) size of buffer, converted from bit to char size (1 for 4)
	 */
	struct s_clientBodyBufferSize
	{
		int		bits;
		int		size;
	};

	/**
	 * @param codes (std::vector<<std::string>>), error codes to be redirected
	 * @param path (std::string), relative path from website directory to error page
	 */
	struct s_errorPage
	{
		std::vector<std::string>	codes;
		std::string					path;
	};
	
	/**
	 * @param ip (uint32_t), INADDR_ANY by default
	 * @param port (uint16_t), 80 by default
	 */
	struct s_listen
	{
		uint32_t	ip;
		uint16_t	port;
	};

	class	IncorrectConfig : virtual public std::exception
	{
		private:
			const char *e;
		public:
			IncorrectConfig(void);
			IncorrectConfig(const char *_e) : e(_e) {};
			IncorrectConfig(IncorrectConfig const &copy);
			IncorrectConfig &operator=(IncorrectConfig const &newObject);
			virtual const char* what() const throw() { return e; }
	};
};

class Parse : public ParseTypedef
{
	private:
		Regex			Regex;
		optionsVec		options;
		locationsVec	locations;
		std::string		configFilePath;
		std::ifstream	stream;
	private:
		Parse();
		Parse(const Parse *);
		Parse operator=(const Parse *);
	#pragma region Read config file and parse in a resiliente way
	public:
		Parse(std::string configFilePath) : configFilePath(configFilePath)
		{
			optionsVec locationTemp;
			std::string line;
			// bool isLocationBlock = false;

			stream.open(configFilePath.c_str());
			stream.exceptions(std::ifstream::badbit);
			if (!stream.is_open())
				throw std::ifstream::failure("Open configuration file failed");
			while (getline(stream, line))
			{
				if (line.empty())
					continue;
				for (int i = 0; i < REGEX_SIZE; i++)
				{
					Regex.matchOne(line, REGEX[i].regex);
					
					for (size_t x = 0; x < Regex.size(); x++)
					{
						std::cout << ">>> " << REGEX[i].name << std::endl;
						std::cout << "Start:" << Regex.match()[x].start << std::endl;
						std::cout << "End:" << Regex.match()[x].end << std::endl;
						std::cout << "Width:" << Regex.match()[x].width << std::endl;
						std::cout << "Occurence:" << Regex.match()[x].occurence << std::endl;
						std::cout << "===================================" << std::endl;
					}
					
					/*if (Regex.match()[0].occurence.size() <= 0)
						continue;
					if (REGEX[i].name == "endLocation")
					{
						locations.push_back(locationTemp);
						locationTemp.clear();
						isLocationBlock = false;
					}
					else if (REGEX[i].name == "location" || isLocationBlock)
					{
						if (!isLocationBlock)
							isLocationBlock = true;
						defaultVector ret;
						for (size_t m = 1; m < Regex.size(); m++)
							if (!Regex.match()[m].occurence.empty())
								ret.push_back(trim(Regex.GetMatch()[m].occurence));
						stringPair newPair = std::make_pair(REGEX[i].name, ret);

						if (REGEX[i].noDuplication)
						{
							optionsIt it = find(locationTemp.begin(), locationTemp.end(), newPair);
							if (it != locationTemp.end())
								locationTemp.erase(it);
						}
						locationTemp.push_back(std::make_pair(REGEX[i].name, ret));
					}
					else
					{
						defaultVector ret;
						for (size_t m = 1; m < Regex.GetSize(); m++)
							if (!Regex.GetMatch()[m].occurence.empty())
								ret.push_back(trim(Regex.GetMatch()[m].occurence));
						stringPair newPair = std::make_pair(REGEX[i].name, ret);

						if (REGEX[i].noDuplication)
						{
							optionsIt it = find(options.begin(), options.end(), newPair);
							if (it != options.end())
								options.erase(it);
						}
						options.push_back(std::make_pair(REGEX[i].name, ret));
					}
					break;*/
				}
			}
			stream.close();
		}
	private:
		inline const optionsIt find(optionsIt first, optionsIt last, stringPair pair)
		{
			while (first != last)
			{
				if (first->first == pair.first)
					return first;
				++first;
			}
			return last;
		}

		std::string& trim(std::string& str, std::string chars = " \t\n\r\f\v")
		{
			str.erase(0, str.find_first_not_of(chars)); // left
			str.erase(str.find_last_not_of(chars) + 1); // right
			return str;
		}
	#pragma endregion Read config file and parse in a resiliente way

	#pragma region Getter
	private:
		defaultVector findKey(std::string key, optionsVec toSearch)
		{
			for (optionsIt it = toSearch.begin(); it != toSearch.end(); it++)
				if (it->first == key)
					return it->second;
			return defaultVector();
		}
	public:
		/// Get vector of general options
		optionsVec		getOptions() { return options; };
		/// Get vector of locations blocks
		locationsVec	getLocations() { return locations; };
		/// Get vector of options for specific locations blocks, search with location_match
		optionsVec		getSpecificLocations(std::string location_match )
		{
			for (locationsIt it = locations.begin(); it != locations.end(); it++)
			{
				defaultVector selectedOption = findKey("location", *it);
				if (!selectedOption.empty() && selectedOption[0] == location_match)
					return *it;
			}
			return optionsVec();
		}
	public:
		std::string	alias(optionsVec vec)
		{
			defaultVector get = findKey("alias", vec);
			if (get.empty())
				return "";
			return get[0];
		}

		s_allow	allow(optionsVec vec)
		{
			defaultVector get = findKey("allow", vec);
			s_allow allow;
			std::string err;

			allow.DELETE = false;
			allow.GET = false;
			allow.POST = false;
			allow.PUT = false;
			err = "rule 'allow': flag ";

			/*Regex.matchAll(get[0], "([[:alnum:]]+)");

			std::cout << get[0] << "-" << Regex.GetSize() << std::endl;
			
			for (size_t x = 1; x < Regex.GetSize(); x++)
			{
				std::string occ = Regex.GetMatch()[x].occurence;
				std::cout << ">>" << occ << std::endl;
				if (occ == "DELETE")
				{
					if (allow.DELETE)
					{
						err += "DELETE is already defined";
						IncorrectConfig(err.c_str());
					}
					allow.DELETE = true;
				}
				else if (occ == "GET")
				{
					if (allow.GET)
					{
						err += "GET is already defined";
						IncorrectConfig(err.c_str());
					}
					allow.GET = true;
				}
				else if (occ == "POST")
				{
					if (allow.POST)
					{
						err += "POST is already defined";
						IncorrectConfig(err.c_str());
					}
					allow.POST = true;
				}
				else if (occ == "PUT")
				{
					if (allow.PUT)
					{
						err += "PUT is already defined";
						IncorrectConfig(err.c_str());
					}
					allow.PUT = true;
				}
				else
				{
					err += occ;
					err += " is undefined";
					IncorrectConfig(err.c_str());
				}
			}*/
			return allow;
		}
	#pragma endregion Getter
	#pragma region Print for debug
	public:
		inline void printConfig()
		{
			int x = 0;

			for (optionsIt it = options.begin(); it != options.end(); it++)
			{
				std::cout << it->first << "=";
				printVector(it->second);
			}
			std::cout << "BLOCK =============" << std::endl;
			for (locationsIt it = locations.begin(); it != locations.end(); it++)
			{
				std::cout << "==  " << x++ << "  =============" << std::endl;
				for (optionsIt it2 = it->begin(); it2 != it->end(); it2++)
				{
					std::cout << it2->first << "=";
					printVector(it2->second);
				}
				std::cout << "=======================" << std::endl;
			}
			std::cout << "END_BLOCK =============" << std::endl;
		}

		inline void printVector(defaultVector vector)
		{
			defaultVector::const_iterator first = vector.begin();
			defaultVector::const_iterator last = vector.end();

			std::cout << "[";
			while (first != last)
			{
				std::cout << *first;
				++first;
				if (first != last)
					std::cout << ", ";
			}
			std::cout << "]" << std::endl;
		}
	#pragma endregion Print for debug
};

#endif
