#ifndef __PARSE
# define __PARSE
# define REGEX_SIZE 14
# define NO_KEY "NO_KEY"
# include <algorithm>
# include <iostream>
# include <sstream>
# include <fstream>
# include <cstring>
# include <cctype>
# include <cstdio>
# include <stdexcept>
# include <map>
# include <vector>
# include <string>
# include <bits/stdc++.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include "Regex.hpp"
# include "nullptr_t.hpp"
# include "Split.hpp"
# include "File.hpp"
# include "http.hpp"

struct s_defineRegex
{
	std::string		name;
	std::string		regex;
	bool			noDuplication;
} REGEX[REGEX_SIZE] =
{
	{ "server", "^[ \t]*(server)[ \t]*\\{?[ \t]*$", false },
	{ "location", "^[ \t]*location[ \t]+([a-zA-Z0-9_/. \t]*)\\{*$", false},
	{ "endBlock", "^[ \t]*(});*$", false },
	/* Rules regex */
	{ "allow", "^[ \t]*allow[ \t]+(.*);[ \t]*$", true },
	{ "autoindex", "^[ \t]*autoindex[ \t]+([a-zA-Z0-9_.\\/\\ ]*);[ \t]*$", true },
	{ "cgi", "^[ \t]*cgi[ \t]+([a-zA-Z0-9_. \t]*)[ \t](\\.?\\/[-a-zA-Z0-9_\\/._]*)[ \t]*(.*);[ \t]*$", true },
	{ "client_body_buffer_size", "^[ \t]*client_body_buffer_size[ \t]+(-?[0-9]+)(b|k|m|g);[ \t]*$", true },
	{ "error_page", "^[ \t]*error_page[ \t]+([0-9x \t]*)(\\/.*);[ \t]*$", false },
	{ "index", "^[ \t]*index[ \t]+(.*);[ \t]*$", true },
	{ "listen", "^[ \t]*listen[ \t]+(.+);[ \t]*$", false },
	{ "return", "^[ \t]*return[ \t]+([a-zA-Z0-9_.\\/]+)[ \t]+(.*);[ \t]*$", true },
	{ "root", "^[ \t]*root[ \t]+(\\.?\\/.*);[ \t]*$", true },
	{ "server_name", "^[ \t]*server_name[ \t]+([-a-zA-Z0-9. \t]*);[ \t]*$", true },
	{ "upload", "^[ \t]*upload[ \t]+(\\.?\\/[-a-zA-Z0-9_\\/._]+)([ \t]+([\\/.][-a-zA-Z0-9_\\/._]+))?;[ \t]*$", true}
};

struct ParseTypedef
{
	struct s_allow
	{
		bool isDefined;
		bool GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, ALL;
	};

	struct s_autoindex
	{
		bool isDefined;
		bool active;
	};

	struct s_cgi
	{
		bool						isDefined;
		std::vector<std::string>	extensions;
		std::string					path;
		s_allow						allow;
	};

	struct s_clientBodyBufferSize
	{
		bool	isDefined;
		size_t	bits;
		size_t	size;
	};
	
	struct s_listen
	{
		uint32_t	ip;
		uint16_t	port;
		std::string ipSave;
		int			portSave;
	};

	struct	s_return
	{
		http_code_t	code;
		std::string	url;
	};

	typedef std::vector<std::string>				stringVector;
	typedef std::pair<std::string, stringVector>	pairOptions;
	typedef std::vector<pairOptions>				optionsMap;
	typedef std::pair<std::string, optionsMap>		pairLocations;
	typedef std::map<std::string, optionsMap>		locationsMap;
	struct s_server
	{
		int				id;
		optionsMap		options;
		locationsMap	locations;
		s_server() : id(-1) {};
	};
	typedef std::vector<s_server>					serversVector;
	typedef std::map<int, std::string>				mapErrors;
	typedef std::vector<s_listen>					mapListens;
	bool											isDefaultLocation;

	class	IncorrectConfig : virtual public std::exception
	{
		private:
			const std::string e;
		public:
			IncorrectConfig();
			IncorrectConfig(const IncorrectConfig &copy);
			IncorrectConfig &operator=(const IncorrectConfig &newObject);
			IncorrectConfig(const std::string &_e) : e(_e) {};
			~IncorrectConfig() throw() {};
			const char* what() const throw() { return e.c_str(); }
	};
};

class Parse : public ParseTypedef
{
	private:
		Regex			Regex, expand;
		serversVector	servers;
		std::string		configPath;
		std::string		configDirectory;
		std::ifstream	stream;
	private:
		Parse(const Parse *);
		Parse operator=(const Parse *);
	#pragma region Read config file and parse in a resiliente way
	private:
		std::string createString(int lineNumber = -1, std::string str = "")
		{
			std::string err("");
			std::stringstream strstream;
			strstream << lineNumber;

			err.append(COLOR_ARGS);
			err.append("[");
			err.append(configPath);
			err.append("]");
			err.append(COLOR_RESET);
			if (lineNumber != -1)
			{
				err.append(COLOR_WARNING);
				err.append(" line ");
				err.append("\x1b[4m");
				err.append(strstream.str());
				err.append(COLOR_RESET);
			}
			err.append(" > ");
			err.append(str);
			return err;
		}

		void generateParseError(int lineNumber = -1, std::string str = "")
		{
			throw IncorrectConfig(createString(lineNumber, str));
		}
	public:
		Parse() {}
		Parse(std::string configFilePath) : configPath(configFilePath) { init(configPath); }
		
		void init(std::string configFilePath, bool setDefaultLocation = true, bool sendInfo = false)
		{
			pairLocations	locationTemp;
			s_server		serverTemp;
			std::string		line;
			int				lineNumber = 0;
			bool asServerBlock = false;
			bool isServerBlock = false, isLocationBlock = false;

			isDefaultLocation = setDefaultLocation;
			configPath = configFilePath;
			configDirectory = relativeToAbsoluteDir(configPath);
			serverTemp.id = 0;
			stream.open(configPath.c_str());
			stream.exceptions(std::ifstream::badbit);
			if (!stream.is_open())
				throw std::ifstream::failure("Open configuration file failed");
			while (getline(stream, line))
			{
				++lineNumber;
				size_t commentPos = line.find("#", 0);
				if (commentPos != std::string::npos)
					line = line.substr(0, commentPos);
				line = trim(line);
				if (isEmpty(line))
					continue;
				for (int i = 0; i < REGEX_SIZE; i++)
				{
					Regex.exec(line, REGEX[i].regex, GLOBAL_FLAG);
					if (Regex.size() == 0)
					{
						if (i == (REGEX_SIZE - 1) && trim(line) != "{" && trim(line) != "}" && sendInfo)
							std::cerr << COLOR_TITLE << "WEBSERV" << COLOR_RESET << " - " << createString(lineNumber, "no valid rule was recognized, type \x1b[92mwebserv \x1b[94m--help\x1b[0m for get more information about the correct format of the rules") << std::endl;
						continue;
					}
					if (REGEX[i].name == "endBlock")
					{
						if (!isLocationBlock && !isServerBlock)
							generateParseError(lineNumber, "a close bracket is desperately alone, it needs a companion");
						if (isLocationBlock && isServerBlock)
						{
							serverTemp.locations.insert(locationTemp);
							locationTemp.first.clear();
							locationTemp.second.clear();
							isLocationBlock = false;
						}
						else
						{
							servers.push_back(serverTemp);
							serverTemp.locations.clear();
							serverTemp.options.clear();
							++serverTemp.id;
							isLocationBlock = false;
							isServerBlock = false;
						}
					}
					else if (REGEX[i].name == "location")
					{
						if (isLocationBlock)
							generateParseError(lineNumber, "a location block cannot contain another one");
						isLocationBlock = true;
						std::string location = trim(Regex.match()[0].occurence);
						locationTemp.first.clear();
						locationTemp.second.clear();
						if (location.empty())
							generateParseError(lineNumber, "a location block must at least contain the path '/'");
						Regex.exec(location, "([a-zA-Z0-9_\\/.]+)", GLOBAL_FLAG);
						if (Regex.size() > 1)
							generateParseError(lineNumber, "only one path is allowed");
						if (serverTemp.locations.find(location) != serverTemp.locations.end())
							generateParseError(lineNumber, "this location block has already been defined");
						locationTemp.first.append(location);
					}
					else if (REGEX[i].name == "server")
					{
						if (isServerBlock)
							generateParseError(lineNumber, "a server block cannot contain another one");
						asServerBlock = true;
						isServerBlock = true;
					}
					else
					{
						if (!isServerBlock)
							generateParseError(lineNumber, "no server block is present. A configuration must be in at least one server block");
						if (isLocationBlock && REGEX[i].name == "error_page")
							generateParseError(lineNumber, "rule error_page is forbidden inside a location block, it must only be located at the root of server block");
						stringVector ret;
						for (size_t m = 0; m < Regex.size(); m++)
							if (!Regex.match()[m].occurence.empty())
								ret.push_back(trim(Regex.match()[m].occurence));
						pairOptions newPair = std::make_pair(REGEX[i].name, ret);
						if (REGEX[i].noDuplication)
						{
							if (isLocationBlock && isServerBlock)
							{
								optionsMap::iterator it = find(locationTemp.second.begin(),  locationTemp.second.end(), newPair);
								if (it != locationTemp.second.end())
								{
									std::string err("rule ");
									err.append(REGEX[i].name);
									err.append(" is not allowed to be written several times in the same location block or at the root of a server block");
									generateParseError(lineNumber, err);
								}
							}
							else
							{
								optionsMap::iterator it = find(serverTemp.options.begin(),  serverTemp.options.end(), newPair);
								if (it != serverTemp.options.end())
								{
									std::string err("rule ");
									err.append(REGEX[i].name);
									err.append(" is not allowed to be written several times in the same location block or at the root of a server block");
									generateParseError(lineNumber, err);
								}
							}
						}
						if (isLocationBlock)
							locationTemp.second.push_back(newPair);
						else
							serverTemp.options.push_back(newPair);
					}
					break;
				}
			}
			stream.close();
			if (!asServerBlock)
				generateParseError(lineNumber, "no server block is present. A configuration must be in at least one server block");
			if (!setDefaultLocation)
				return;
			for (serversVector::iterator it = servers.begin(); it != servers.end(); it++)
			{
				pairLocations	root;
				locationsMap::iterator find = (*it).locations.find("/");

				if (find != (*it).locations.end())
					root = *find;
				root.first = "/";
				if ((*it).options.size() <= 0 && (*it).locations.size() <= 0)
					generateParseError(-1, "no option(s) or location block(s) is present. A configuration must be in at least one option or one location block");
				for (optionsMap::iterator itConf = (*it).options.begin(); itConf != (*it).options.end(); itConf++)
				{
					if ((*itConf).first == "listen" || (*itConf).first == "server_name" || (*itConf).first == "client_body_buffer_size" || (*itConf).first == "error_page")
						continue;
					root.second.push_back((*itConf));
					(*it).options.erase(itConf);
					if ((*it).options.size() <= 0)
						break;
					itConf = (*it).options.begin();
				}
				if (find != (*it).locations.end())
					(*it).locations.erase(find);
				(*it).locations.insert((*it).locations.begin(), root);
			}
		}
	private:
		bool isEmpty(std::string str)
		{
			if (str.empty())
				return true;
			for (std::string::iterator it = str.begin(); it != str.end(); it++)
				if (!isspace(*it))
					return false;
			return true;
		}

		inline const optionsMap::iterator find(optionsMap::iterator first, optionsMap::iterator last, pairOptions pair)
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
	public:
		/**
		 * Get stringVector of arguments of key element
		 * @param key (std::string) key of element (alias, allow, ...)
		 * @param toSearch (optionsMap) vector of options
		 * @return stringVector of arguments, if not exist, first element of 
		 * vector is set to `NO_KEY` value
		 */
		stringVector	findKey(std::string key, optionsMap toSearch)
		{
			for (optionsMap::iterator it = toSearch.begin(); it != toSearch.end(); it++)
				if (it->first == key)
					return it->second;
			return stringVector(1, NO_KEY);
		}

		/**
		 * Test if file exist
		 * @param name (std::string) path to file
		 * @return true if exist
		 */
		bool 			exist(const std::string name)
		{
			struct stat buf;
			int ret = stat(name.c_str(), &buf);

			if (ret == 0 && S_ISREG(buf.st_mode) == 1)
				return true;
			return false;
		}
	public:
		/**
		 * Get vector of s_server
		 * @return std::vector<s_server> (serversVector)
		 */
		serversVector	getServers() { return servers; }

		/**
		 * Get specific server block by ID
		 * @param id ID of server block, 0 by default (first block)
		 * @return s_server of selected block, or empty s_server if error
		 */
		s_server		getServerBlock(int id = 0)
		{
			if (!servers.empty())
				for (serversVector::iterator it = servers.begin(); it != servers.end(); it++)
					if ((*it).id == id)
						return *it;
			return s_server();
		}
		
		/**
		 * Get specific block location of server block
		 * @param server server block
		 * @param locationMatch	path of location block search
		 */
		optionsMap	getSpecificLocation(s_server server, std::string locationMatch)
		{
			for (locationsMap::iterator it = server.locations.begin(); it != server.locations.end(); it++)
				if ((*it).first == locationMatch)
					return (*it).second;
			return optionsMap();
		}

	private:
		void _allow(std::string str, std::string err, s_allow *allow)
		{
			Regex.exec(str, "([-a-zA-Z0-9_]+)", GLOBAL_FLAG);
			for (size_t x = 0; x < Regex.size(); x++)
			{
				std::string occ = Regex.match()[x].occurence;
				if (occ == "GET")
				{
					if (allow->GET)
					{
						err += "GET is already defined";
						throw IncorrectConfig(err);
					}
					allow->GET = true;
				}
				else if (occ == "HEAD")
				{
					if (allow->HEAD)
					{
						err += "HEAD is already defined";
						throw IncorrectConfig(err);
					}
					allow->HEAD = true;
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
				else if (occ == "DELETE")
				{
					if (allow->DELETE)
					{
						err += "DELETE is already defined";
						throw IncorrectConfig(err);
					}
					allow->DELETE = true;
				}
				else if (occ == "CONNECT")
				{
					if (allow->CONNECT)
					{
						err += "CONNECT is already defined";
						throw IncorrectConfig(err);
					}
					allow->CONNECT = true;
				}
				else if (occ == "OPTIONS")
				{
					if (allow->OPTIONS)
					{
						err += "OPTIONS is already defined";
						throw IncorrectConfig(err);
					}
					allow->OPTIONS = true;
				}
				else if (occ == "TRACE")
				{
					if (allow->TRACE)
					{
						err += "TRACE is already defined";
						throw IncorrectConfig(err);
					}
					allow->TRACE = true;
				}
				else if (occ == "ALL")
				{
					if (allow->ALL)
					{
						err += "ALL is already defined";
						throw IncorrectConfig(err);
					}
					allow->ALL = true;
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
		s_allow	allow(optionsMap vec)
		{
			stringVector	get = findKey("allow", vec);
			std::string		err = "rule 'allow': flag ";
			s_allow			allow;
			
			allow.isDefined = true;
			allow.GET = false;
			allow.HEAD = false;
			allow.POST = false;
			allow.PUT = false;
			allow.DELETE = false;
			allow.CONNECT = false;
			allow.OPTIONS = false;
			allow.TRACE = false;
			allow.ALL = false;
			if (get[0] == NO_KEY)
			{
				allow.isDefined = false;
				return allow;
			}
			if (get.empty())
				throw IncorrectConfig("rule 'allow': no argument is set");
			_allow(get[0], "rule 'allow': flag ", &allow);
			return allow;
		}

		s_autoindex autoindex(optionsMap vec)
		{
			stringVector get = findKey("autoindex", vec);
			s_autoindex autoindex;
			std::string err = "rule 'autoindex': ";

			autoindex.isDefined = false;
			autoindex.active = false;
			if (!get.empty() && get[0] != NO_KEY)
			{
				std::string temp = trim(get[0]);
				autoindex.isDefined = true;
				if (get.size() > 1)
				{
					err += "there can be only one argument";
					throw IncorrectConfig(err);
				}
				if (temp != "on" && temp != "off")
				{
					err += "the argument can only be 'on' or 'off', not ";
					err.append(temp);
					throw IncorrectConfig(err);
				}
				autoindex.active = (temp == "off") ? false : true;
			}
			return autoindex;
		}
	
		s_cgi cgi(optionsMap vec)
		{
			stringVector get = findKey("cgi", vec);
			s_cgi cgi;
			std::string err = "rule 'cgi': ";
			
			cgi.isDefined = true;
			if (get.size() == 1 && get[0] == "NO_KEY")
			{
				cgi.isDefined = false;
				return cgi;
			}
			if (get.size() != 3)
			{
				err += "there must be at least [extension] [path to executable] [http request allowed]";
				throw IncorrectConfig(err.c_str());
			}
			Regex.exec(get[0], "(\\.[a-zA-Z0-9_]+)", GLOBAL_FLAG);
			for (size_t x = 0; x < Regex.size(); x++)
				cgi.extensions.push_back(Regex.match()[x].occurence);

			cgi.path = concatPath(configDirectory, get[1]);
			if (!exist(cgi.path))
			{
				err += "the executable does not exist";
				throw IncorrectConfig(err);
			}

			cgi.allow.DELETE = false;
			cgi.allow.GET = false;
			cgi.allow.HEAD = false;
			cgi.allow.POST = false;
			cgi.allow.PUT = false;
			cgi.allow.DELETE = false;
			cgi.allow.CONNECT = false;
			cgi.allow.OPTIONS = false;
			cgi.allow.TRACE = false;
			cgi.allow.ALL = false;
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
		s_clientBodyBufferSize clientBodyBufferSize(optionsMap vec)
		{
			stringVector get = findKey("client_body_buffer_size", vec);
			s_clientBodyBufferSize client;

			client.bits = 16000;
			client.size = 2000;
			client.isDefined = false;
			if (!get.empty() && get[0] != NO_KEY)
			{
				client.isDefined = true;
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
	
	private:
		int generate(int hundred, int ten, int unit)
		{
			int ret = 0;
			ret += hundred * 100;
			ret += ten * 10;
			ret += unit;
			return ret;
		}
	public:
		mapErrors errorPage(optionsMap vec)
		{
			std::vector<stringVector>	errorsList;
			mapErrors					errors;
			int							hundred, ten, unit;

			for (optionsMap::iterator it = vec.begin(); it != vec.end(); it++)
				if (it->first == "error_page")
					errorsList.push_back(it->second);
			if (errorsList.size() > 0)
			{
				for (std::vector<stringVector>::iterator page = errorsList.begin(); page != errorsList.end(); page++)
				{
					Regex.exec((*page)[0], "(-?[0-9x]+)", GLOBAL_FLAG);
					for (size_t x = 0; x < Regex.size(); x++)
					{
						expand.exec(Regex.match()[x].occurence, "^([0-9]|x)([0-9]|x)([0-9]|x)$", GLOBAL_FLAG);
						hundred	= (expand.match()[0].occurence != "x") ? std::atoi(expand.match()[0].occurence.c_str()) : -1;
						ten 	= (expand.match()[1].occurence != "x") ? std::atoi(expand.match()[1].occurence.c_str()) : -1;
						unit 	= (expand.match()[2].occurence != "x") ? std::atoi(expand.match()[2].occurence.c_str()) : -1;
						if (hundred != -1 && ten != -1 && unit != -1)
							errors.insert(std::pair<int,std::string>(std::atoi(Regex.match()[x].occurence.c_str()), trim((*page)[1])));
						else
							for (int one = (hundred == -1) ? 1 : hundred; one <= ((hundred == -1) ? 5 : hundred); one++)
								for (int two = (ten == -1) ? 0 : ten; two <= ((ten == -1) ? 9 : ten); two++)
									for (int three = (unit == -1) ? 0 : unit; three <= ((unit == -1) ? 9 : unit); three++)
										errors.insert(std::pair<int,std::string>(generate(one, two, three), trim((*page)[1])));
					}
				}
			}
			return errors;
		}

		std::string index(optionsMap vec)
		{
			stringVector	get = findKey("index", vec);
			std::string		ret = "index.html";

			if (get[0] == NO_KEY)
				return ret;
			Regex.exec(get[0], "([-a-zA-Z0-9_.]+)", GLOBAL_FLAG);
			if (Regex.size() > 1)
				throw IncorrectConfig("rule 'allow': only one file definition is allowed");
			return Regex.match()[0].occurence;
		}

	private:
		bool		ipIsValid(std::string ip)
		{
			if (ip == "localhost")
				return true;
			s_split	_split = split(ip, ".");
			long 	temp;

			if (_split.size() != 4)
				return false;
			for (s_split::const_iterator it = _split.begin(); it != _split.end(); it++)
			{
				temp = std::atol((*it).c_str());
				if (!isNumber((*it)) || temp < 0 || temp > 255)
					return false;
			}
			return true;
		}
	public:
		mapListens	listen(optionsMap vec)
		{
			std::vector<std::string>	listenList;
			mapListens					listens;

			for (optionsMap::iterator it = vec.begin(); it != vec.end(); it++)
				if (it->first == "listen")
				{
					std::string temp = trim(it->second[0]);
					if (std::find(listenList.begin(), listenList.end(), temp) == listenList.end())
						listenList.push_back(trim(it->second[0]));
					else
					{
						std::string c = "rule 'listen': ";
						c += temp;
						c += " is defined several times inside same server block";
						throw IncorrectConfig(c.c_str());
					}
				}
			if (listenList.size() > 0)
			{
				for (std::vector<std::string>::iterator listen = listenList.begin(); listen != listenList.end(); listen++)
				{
					std::string	ip = "0.0.0.0";
					int			port = 80;
					s_listen	ret;
					Regex.exec(*listen, "^([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|[a-zA-Z_-]+|[0-9]+):?(-?[0-9]+)?$", GLOBAL_FLAG);
					if (Regex.size() == 2)
					{
						ip = Regex.match()[0].occurence;
						port = std::atol(Regex.match()[1].occurence.c_str());
					}
					else if (Regex.size() == 1)
					{
						std::string temp = Regex.match()[0].occurence;
						Regex.exec(temp, "([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|[a-zA-Z_-]+)", GLOBAL_FLAG);
						if (Regex.size() > 0)
							ip = temp;
						else
							port = std::atol(temp.c_str());
					}
					else
						throw IncorrectConfig("rule 'listen': there is a formatting error. If you want to define more than one ip or port, you must have as many listen rules");
					ret.ipSave = ip;
					ret.portSave = port;
					ret.ip = static_cast<size_t>(inet_addr(ip.c_str()));
					if (!ipIsValid(ip))
					{
						std::string c = "rule 'listen': ip address ";
						c += ip.c_str();
						c += " is outside the range [0.0.0.0] <> [255.255.255.255]";
						throw IncorrectConfig(c.c_str());
					}
					if (port < 0 || port > 65535)
					{
						std::string c = "rule 'listen': port ";
						std::stringstream strstream;
						strstream << port;
						c += strstream.str();
						c += " is outside the range [0] <> [65535]";
						throw IncorrectConfig(c.c_str());
					}
					ret.port = static_cast<size_t>(htons(port));
					listens.push_back(ret);
				}
			}
			return listens;
		}

		s_return	_return(optionsMap vec)
		{
			stringVector	get = findKey("return", vec);
			s_return		ret;
			
			if (get[0] != NO_KEY)
			{
				int number = std::atoi(get[0].c_str());
				if (number == C_UNKNOWN ||
				(number >= C_CONTINUE && number <= C_SWITCHING_PROTOCOLS) ||
				(number >= C_OK && number <= C_PARTIAL_CONTENT) ||
				(number >= C_MULTIPLE_CHOICE && number <= C_PERMANENT_REDIRECT && number != 306) ||
				(number >= C_BAD_REQUEST && number <= C_IM_A_TEAPOT) ||
				(number >= C_INTERNAL_SERVER_ERROR && number <= C_HTTP_VERSION_NOT_SUPPORTED))
					ret.code = static_cast<http_code_t>(number);
				else
				{
					std::string c = "rule 'return': http code ";
					c += get[0].c_str();
					c += " is incorrect";
					throw IncorrectConfig(c.c_str());
				}
				ret.url = get[1];
			}
			return ret;
		}

		std::string root(optionsMap vec)
		{
			stringVector	get = findKey("root", vec);
		
			if (get[0] == NO_KEY)
				return "";
			Regex.exec(get[0], "([-a-zA-Z0-9_\\./\\]+)", GLOBAL_FLAG);
			if (Regex.size() > 1)
				throw IncorrectConfig("rule 'root': only one directory definition is allowed");
			return concatPath(configDirectory, Regex.match()[0].occurence);
		}

		stringVector serverName(optionsMap vec)
		{
			stringVector	get = findKey("server_name", vec);
			stringVector	ret;

			if (get[0] != NO_KEY)
			{
				Regex.exec(get[0], "([a-zA-Z0-9_.]+)", GLOBAL_FLAG);
				for (size_t x = 0; x < Regex.size(); x++)
					ret.push_back(Regex.match()[x].occurence);
			}
			return ret;
		}

		std::pair<std::string, std::string> upload(optionsMap vec)
		{
			stringVector	get = findKey("upload", vec);

			if (get[0] == NO_KEY)
				return (std::make_pair("", ""));
			else if (get[1] != NO_KEY && get[2] != NO_KEY)
				return (std::make_pair(concatPath(configDirectory, get[0]), get[2]));
			else
				return (std::make_pair(concatPath(configDirectory, get[0]), ""));
		}
	#pragma endregion Getter
	
	#pragma region Check config
	public:
		inline void check(bool defaultLocation = true)
		{
			std::vector<mapListens>		checkListen;
			std::vector<std::string>	checkServer;
			std::vector<std::string>	tempServer;
			int							count = 0;

			for (serversVector::const_iterator it = servers.begin(); it != servers.end(); it++)
			{
				checkServer.clear();
				if (!defaultLocation)
				{
					allow((*it).options);
					autoindex((*it).options);
					cgi((*it).options);
					errorPage((*it).options);
					index((*it).options);
					_return((*it).options);
					root((*it).options);
					upload((*it).options);
				}
				clientBodyBufferSize((*it).options);
				tempServer = serverName((*it).options);
				listen((*it).options);
				
				for (std::vector<std::string>::const_iterator itC = tempServer.begin(); itC != tempServer.end(); itC++)
					checkServer.push_back(*itC);
				for (std::vector<std::string>::const_iterator it = checkServer.begin(); it != checkServer.end(); it++)
				{
					count = std::count(checkServer.begin(), checkServer.end(), *it);
					if (count > 1)
					{
						std::string c = "rule 'server_name': ";
						c += *it;
						c += " has several definitions and therefore cannot work properly";
						throw IncorrectConfig(c.c_str());
					}
				}
				
				if (!((*it).locations.empty()))
				{
					for (locationsMap::const_iterator itLoc = (*it).locations.begin(); itLoc != (*it).locations.end(); itLoc++)
					{
						allow((*itLoc).second);
						autoindex((*itLoc).second);
						cgi((*itLoc).second);
						clientBodyBufferSize((*itLoc).second);
						errorPage((*itLoc).second);
						index((*itLoc).second);
						_return((*itLoc).second);
						root((*itLoc).second);
						upload((*it).options);
					}
				}
			}
		}
	#pragma endregion Check config
	
	#pragma region Print for debug
	public:
		/**
		 * Print to std::cout parsed configuration file to json format
		 */
		inline void print()
		{
			int size;
			std::string TAB = "  ", SEP = "\"";
			
			std::cout << "[" << std::endl;
			for (serversVector::iterator it = servers.begin(); it != servers.end(); )
			{
				std::cout << TAB << "{" << std::endl;
				std::cout << TAB << TAB << SEP << "ID" << SEP << ": " << SEP << (*it).id << SEP << "," << std::endl;
				size = (*it).options.size();
				if (!(*it).locations.empty())
					size = -1;
				for (optionsMap::iterator itConf = (*it).options.begin(); itConf != (*it).options.end(); itConf++)
					printArgs(*itConf, false, size--);
				if (!(*it).locations.empty())
				{
					std::cout << TAB << TAB << SEP << "location" << SEP << ": {" << std::endl;
					locationsMap::iterator itLocBegin = (*it).locations.begin();
					locationsMap::iterator itLocEnd = (*it).locations.end();
					while (itLocBegin != itLocEnd)
					{
						size = (*itLocBegin).second.size();
						std::cout << TAB << TAB << TAB << SEP << (*itLocBegin).first << SEP << ": {" << std::endl;
						for (optionsMap::iterator itConf = (*itLocBegin).second.begin(); itConf != (*itLocBegin).second.end(); itConf++)
							printArgs(*itConf, true, size--);
						++itLocBegin;
						std::cout << TAB << TAB << TAB << "}";
						if (itLocBegin != itLocEnd)
							std::cout << ",";
						std::cout << std::endl;
					}
					std::cout << TAB << TAB << "}" << std::endl;
				}
				std::cout << TAB << "}";
				++it;
				if (it != servers.end())
					std::cout << ",";
				std::cout << std::endl;
			}
			std::cout << "]" << std::endl;
		}
	private:
		inline void printArgs(pairOptions vector, bool addTab = false, int size = -1)
		{
			std::string TAB = "  ", SEP = "\"";
			stringVector::const_iterator begin = vector.second.begin();
			stringVector::const_iterator end = vector.second.end();

			if (addTab == true)
				std::cout << TAB << TAB;
			std::cout << TAB << TAB << SEP << vector.first << SEP << ": ";
			std::cout << "[";
			while (begin != end)
			{
				std::cout << SEP << *begin << SEP;
				++begin;
				if (begin != end)
					std::cout << ", ";
			}
			std::cout << "]";
			if (size <= -1 || size > 1)
				std::cout << ",";
			std::cout << std::endl;
		}
	#pragma endregion Print for debug
};
#endif
