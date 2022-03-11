#ifndef __PARSE
# define __PARSE
# define REGEX_SIZE 12
# define NO_KEY "NO_KEY"
# include <algorithm>
# include <iostream>
# include <sstream>
# include <fstream>
# include <cstring>
# include <cstdio>
# include <exception>
# include <map>
# include <vector>
# include <string>
# include <bits/stdc++.h>
# include <sys/stat.h>
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
	{ "autoindex", "^[ \t]*autoindex[ \t]+([a-zA-Z0-9_.\\/\\ ]*);$", true },
	{ "cgi", "^[ \t]*cgi[ \t]+([a-zA-Z0-9_. \t]*)(\\/[-a-zA-Z0-9_\\/._]*)[ \t]*(.*);$", true },
	{ "client_body_buffer_size", "^[ \t]*client_body_buffer_size[ \t]+(-?[0-9]+)(b|k|m|g);$", true },
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
		s_allow						allow;							
	};

	/**
	 * @param (int) bit size
	 * @param (int) size of buffer, converted from bit to char size (1 for 4)
	 */
	struct s_clientBodyBufferSize
	{
		long int	bits;
		long int	size;
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
			const std::string e;
		public:
			IncorrectConfig(void);
			IncorrectConfig(const std::string _e) : e(_e) {};
			IncorrectConfig(IncorrectConfig const &copy);
			IncorrectConfig &operator=(IncorrectConfig const &newObject);
			virtual const char* what() const throw() { return e.c_str(); }
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
			bool isLocationBlock = false;

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
					Regex.exec(line, REGEX[i].regex, GLOBAL_FLAG);
					if (Regex.size() == 0)
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
						for (size_t m = 0; m < Regex.size(); m++)
							if (!Regex.match()[m].occurence.empty())
								ret.push_back(trim(Regex.match()[m].occurence));
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
						for (size_t m = 0; m < Regex.size(); m++)
							if (!Regex.match()[m].occurence.empty())
								ret.push_back(trim(Regex.match()[m].occurence));
						stringPair newPair = std::make_pair(REGEX[i].name, ret);

						if (REGEX[i].noDuplication)
						{
							optionsIt it = find(options.begin(), options.end(), newPair);
							if (it != options.end())
								options.erase(it);
						}
						options.push_back(std::make_pair(REGEX[i].name, ret));
					}
					break;
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
			return defaultVector(1, NO_KEY);
		}

		bool exist(const std::string name)
		{
			struct stat buf;
			int ret = stat(name.c_str(), &buf);

			if (ret == 0 && S_ISREG(buf.st_mode) == 1)
				return true;
			return false;
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
			if (get[0] == NO_KEY)
				return std::string();
			if (get.empty())
				throw IncorrectConfig("rule 'alias': no argument is set");
			return get[0];
		}
	private:
		void _allow(std::string str, std::string err, s_allow *allow)
		{
			Regex.exec(str, "([-a-zA-Z0-9_]+)", GLOBAL_FLAG);
			for (size_t x = 0; x < Regex.size(); x++)
			{
				std::string occ = Regex.match()[x].occurence;
				if (occ == "DELETE")
				{
					if (allow->DELETE)
					{
						err += "DELETE is already defined";
						throw IncorrectConfig(err);
					}
					allow->DELETE = true;
				}
				else if (occ == "GET")
				{
					if (allow->GET)
					{
						err += "GET is already defined";
						throw IncorrectConfig(err);
					}
					allow->GET = true;
				}
				else if (occ == "POST")
				{
					if (allow->POST)
					{
						err += "POST is already defined";
						throw IncorrectConfig(err);
					}
					allow->POST = true;
				}
				else if (occ == "PUT")
				{
					if (allow->PUT)
					{
						err += "PUT is already defined";
						throw IncorrectConfig(err);
					}
					allow->PUT = true;
				}
				else
				{
					err += occ;
					err += " is incorrect";
					throw IncorrectConfig(err);
				}
			}
		}
	public:
		s_allow	allow(optionsVec vec)
		{
			defaultVector	get = findKey("allow", vec);
			std::string		err = "rule 'allow': flag ";
			s_allow			allow;
			
			allow.DELETE = false;
			allow.GET = false;
			allow.POST = false;
			allow.PUT = false;
			if (get[0] == NO_KEY)
				return allow;
			if (get.empty())
				throw IncorrectConfig("rule 'allow': no argument is set");
			_allow(get[0], "rule 'allow': flag ", &allow);
			return allow;
		}

		s_autoindex autoindex(optionsVec vec)
		{
			defaultVector get = findKey("autoindex", vec);
			s_autoindex autoindex;
			std::string err = "rule 'autoindex': ";

			autoindex.active = false;
			if (!get.empty() && get[0] != NO_KEY)
			{
				if (get.size() > 1)
				{
					err += "there can be only one argument";
					throw IncorrectConfig(err);
				}
				Regex.exec(get[0], "([-a-zA-Z0-9_]+)", GLOBAL_FLAG);
				if (Regex.size() > 1)
				{
					err += "there can be only one argument";
					throw IncorrectConfig(err);
				}
				if (Regex.match()[0].occurence != "on" && Regex.match()[0].occurence != "off")
				{
					err += "the argument can only be on or off, not ";
					err += Regex.match()[0].occurence;
					throw IncorrectConfig(err);
				}
				autoindex.active = (Regex.match()[0].occurence == "off") ? true : false;
			}
			return autoindex;
		}
		
		s_cgi cgi(optionsVec vec)
		{
			defaultVector get = findKey("cgi", vec);
			s_cgi cgi;
			std::string err = "rule 'cgi': ";
			
			if (get.size() != 3)
			{
				err += "there must be at least [extension] [path to executable] [http request allowed]";
				throw IncorrectConfig(err.c_str());
			}
			Regex.exec(get[0], "(\\.[a-zA-Z0-9_]+)", GLOBAL_FLAG);
			for (size_t x = 0; x < Regex.size(); x++)
				cgi.extensions.push_back(Regex.match()[x].occurence);

			if (!exist(get[1]))
			{
				err += "the executable does not exist";
				throw IncorrectConfig(err);
			}
			cgi.path = get[1];

			cgi.allow.DELETE = false;
			cgi.allow.GET = false;
			cgi.allow.POST = false;
			cgi.allow.PUT = false;
			_allow(get[2], "rule 'cgi': flag ", &cgi.allow);
			return cgi;
		}
	private:
		bool isNumber(const std::string &str)
		{
			for (size_t x = 0; x < str.size(); x++)
				if (std::isdigit(str[x] == 0))
					return false;
			return true;
		}
	public:
		s_clientBodyBufferSize clientBodyBufferSize(optionsVec vec)
		{
			defaultVector get = findKey("client_body_buffer_size", vec);
			s_clientBodyBufferSize client;

			client.bits = 16000;
			client.size = 2000;
			if (!get.empty() && get[0] != NO_KEY)
			{
				long int temp = atol(get[0].c_str());
				if (temp < 8)
				{
					std::string err = "rule 'client_body_buffer_size': the minimum must be 8 bits (one character), actual size is ";
					std::stringstream strstream;
					strstream << temp;
					err += strstream.str();
					throw IncorrectConfig(err);
				}
				if (get[1][0] == 'k')
					temp *= 1000;
				else if (get[1][0] == 'm')
					temp *= 1000000;
				else if (get[1][0] == 'g')
					temp *= 1000000000;
				client.bits = temp;
				client.size = temp / 8;
			}
			return client;
		}

		typedef std::vector<s_errorPage>	errorPageVector;
		errorPageVector errorPage(optionsVec vec)
		{
			std::vector<defaultVector>	errorsList;
			errorPageVector				errors;
			s_errorPage					temp;

			for (optionsIt it = vec.begin(); it != vec.end(); it++)
				if (it->first == "error_page")
					errorsList.push_back(it->second);
			if (errorsList.size() > 0)
			{
				for (std::vector<defaultVector>::iterator page = errorsList.begin(); page != errorsList.end(); page++)
				{
					Regex.exec((*page)[0], "(-?[0-9]+)", GLOBAL_FLAG);
					for (size_t x = 0; x < Regex.size(); x++)
						temp.codes.push_back(Regex.match()[x].occurence);
					temp.path = trim((*page)[1]);
					errors.push_back(temp);
				}
			}
			return errors;
		}

		std::string index(optionsVec vec)
		{
			defaultVector	get = findKey("index", vec);
			std::string		ret = "index.html";

			if (get[0] == NO_KEY)
				return ret;
			Regex.exec(get[0], "([-a-zA-Z0-9_.]+)", GLOBAL_FLAG);
			if (Regex.size() > 1)
				throw IncorrectConfig("rule 'allow': only one file definition is allowed");
			return Regex.match()[0].occurence;
		}

		std::string root(optionsVec vec)
		{
			defaultVector	get = findKey("root", vec);
		
			if (get[0] == NO_KEY)
				throw IncorrectConfig("rule 'root': no rule is defined, the server can't work");
			Regex.exec(get[0], "([-a-zA-Z0-9_./\\]+)", GLOBAL_FLAG);
			if (Regex.size() > 1)
				throw IncorrectConfig("rule 'root': only one directory definition is allowed");
			return Regex.match()[0].occurence;
		}

		std::vector<std::string> server_name(optionsVec vec)
		{
			defaultVector				get = findKey("server_name", vec);
			std::vector<std::string>	ret;

			if (get[0] == NO_KEY)
				ret.push_back("localhost");
			else
			{
				Regex.exec(get[0], "([a-zA-Z0-9_.]+)", GLOBAL_FLAG);
				for (size_t x = 0; x < Regex.size(); x++)
					ret.push_back(Regex.match()[x].occurence);
			}
			return ret;
		}
	public:
		s_listen	listen(optionsVec vec)
		{
			defaultVector				get = findKey("listen", vec);
			std::vector<std::string>	temp;
			std::string					ip = "127.0.0.1";
			int							port = 80;
			s_listen					ret;

			if (get[0] != NO_KEY)
			{
				Regex.exec(get[0], "([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|[a-zA-Z_-]+|[0-9]+):?([0-9]+)?", GLOBAL_FLAG);
				for (size_t x = 0; x < Regex.size(); x++)
				{
					if (Regex.match()[x].occurence.size() == 0)
						continue;
					temp.push_back(Regex.match()[x].occurence);
				}
				for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); it++)
				{
					Regex.exec(ip, "([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|[a-zA-Z_-]+)");
					if (Regex.size() == 1)
						ip = *it;
					else
						port = atoi((*it).c_str());
				}
			}
			ret.ip = inet_addr(ip.c_str());
			ret.port = htons(port);
			return ret;
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
