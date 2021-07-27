#ifndef __PARSE
# define __PARSE
# include <iostream>
# include <fstream>
# include <cstring>
# include <exception>
# include <map>
# include <vector>
# include "Regex.hpp"

struct s_server
{
	std::vector<std::pair<std::string, std::string> >	options;
	std::vector<std::map<std::string, std::string> >	locations;
};

class Parse
{
	private:
		std::string											_configPath;
		std::string											_line;
		std::ifstream										CONFIG;
		Regex												regex;
		std::vector<std::string> 							AutorizedFlag;
		std::vector<std::pair<std::string, std::string> >	ParsingResult;
		std::vector<s_server>								Configuration;
	public:
		Parse(const Parse &x) : _configPath(x._configPath), AutorizedFlag(x.AutorizedFlag), Configuration(x.Configuration) {}
		Parse &operator=(const Parse &x)
		{
			_configPath = x._configPath;
			AutorizedFlag = x.AutorizedFlag;
			Configuration = x.Configuration;
			return *this;
		}
	private:
		Parse(void) {}
		std::string& leftTrim(std::string& str, std::string& chars)
		{
			str.erase(0, str.find_first_not_of(chars));
			return str;
		}
		std::string& rightTrim(std::string& str, std::string& chars)
		{
			str.erase(str.find_last_not_of(chars) + 1);
			return str;
		}
		void fillVector()
		{
			AutorizedFlag.push_back("server_name");
			AutorizedFlag.push_back("listen");
			AutorizedFlag.push_back("root");
			AutorizedFlag.push_back("index");
			AutorizedFlag.push_back("autoindex");
			AutorizedFlag.push_back("cli_max_size");
			AutorizedFlag.push_back("error_page");
			AutorizedFlag.push_back("allow");
			AutorizedFlag.push_back("autoindex");
			AutorizedFlag.push_back("alias");
			AutorizedFlag.push_back("cgi_pass");
		}
	private:
		void fillParsingResultVector()
		{
			std::string key;
			std::string value;
			while (getline(CONFIG, _line))
			{
				if (_line.empty())
					continue ;
				regex.Match(_line, "^[^ ]+");
				key = trim(regex.GetMatch()[0].occurence, 0);
				if (key == "{")
					continue ;
				_line = _line.substr(regex.GetMatch()[0].end, (_line.size() - regex.GetMatch()[0].start));
				regex.Match(_line, "^.*\\b");
				value = regex.GetMatch()[0].occurence;
				value = trim(value);
				ParsingResult.push_back(std::make_pair(key, value));
			}
		}
		
		/**
		 * Parsing system with multi level of depth
		 */
		typedef std::vector<std::pair<std::string, std::string> >::iterator _iterator;
		
		bool keyIsValid(std::string key)
		{
			for (std::vector<std::string>::iterator it = AutorizedFlag.begin(); it != AutorizedFlag.end(); it++)
			{ if (*it == key) return true; }
			return false;
		}

		void parsingLocationBlock(s_server *_block, _iterator begin, _iterator end)
		{
			std::map<std::string, std::string> _location;
			if (begin->first == "location")
			{
				std::string _loc = begin->second;
				if (_loc.length() <= 0) _loc = "/";
				_location.insert(std::make_pair(begin->first, _loc));
			}
			while (begin != end)
			{
				if (keyIsValid(begin->first))
					_location.insert(std::make_pair(begin->first, begin->second));
				begin++;
			}
			_block->locations.push_back(_location);
		}

		void parsingServerBlock(_iterator begin, _iterator End)
		{
			s_server _block;
			while (begin != End)
			{
				if (begin->first == "location")
				{
					_iterator _LocationEndBlock = begin;
					while (_LocationEndBlock != ParsingResult.end())
					{
						if (_LocationEndBlock->first == "}")
							break ;
						_LocationEndBlock++;
					}
					parsingLocationBlock(&_block, begin, _LocationEndBlock);
					begin = _LocationEndBlock;
				}
				if (keyIsValid(begin->first))
					_block.options.push_back(std::make_pair(begin->first, begin->second));
				begin++;
			}
			Configuration.push_back(_block);
		}
		
		void createStructTreeOfConfig()
		{
			bool IsLocationBlock = false;
			for (_iterator it = ParsingResult.begin(); it != ParsingResult.end(); it++)
			{
				if (it->first == "server")
				{
					_iterator _ServerEndBlock = it;
					while (_ServerEndBlock != ParsingResult.end())
					{
						if (!IsLocationBlock && _ServerEndBlock->first == "}")
							break ;
						if (IsLocationBlock && _ServerEndBlock->first == "}")
						{
							IsLocationBlock = false;
							_ServerEndBlock++;
							continue ;
						}
						if (_ServerEndBlock->first == "location")
							IsLocationBlock = true;
						_ServerEndBlock++;
					}
					parsingServerBlock(it, _ServerEndBlock);
					it = _ServerEndBlock;
				}
			}
		}

		void StartParsing()
		{
			fillVector();
			CONFIG.open(_configPath.c_str());
			CONFIG.exceptions(std::ifstream::badbit);
			if (!CONFIG.is_open())
				throw std::ifstream::failure("Open configuration file failed");
			fillParsingResultVector();
			CONFIG.close();
			if (ParsingResult[0].first != "server")
				throw IncorrectConfig("Config file not begin with a new server block");
			createStructTreeOfConfig();
		}
	public:
		Parse(std::string config_path) : _configPath(config_path) { StartParsing(); }
	public:
		/**
		 * 	Get current configuration, iterable with iterator
		 */
		std::vector<s_server> GetConfiguration() { return Configuration; }
		/**
		 * 	Change link of config file, and auto update configuration.
		 * 	Iterator created with last configuration change is invalid.
		 * 	@param config_path: relative link to configuration path
		 */
		void setConfig(std::string config_path)
		{
			_configPath = config_path;
			StartParsing();
		}

		void print()
		{
			for (std::vector<s_server>::iterator it = Configuration.begin(); it != Configuration.end(); it++)
			{
				std::cout << "Global options" << std::endl;
				for (std::vector<std::pair<std::string, std::string> >::iterator it2 = it->options.begin(); it2 != it->options.end(); it2++)
					std::cout << it2->first << "=" << it2->second << std::endl;
				std::cout << "Location block(s)" << std::endl;
				for (std::vector<std::map<std::string, std::string> >::iterator it2 = it->locations.begin(); it2 != it->locations.end(); it2++)
				{
					std::cout << "Location {" << std::endl;
					for (std::map<std::string, std::string>::iterator it3 = it2->begin(); it3 != it2->end(); it3++)
						std::cout << it3->first << "=" << it3->second << std::endl;
					std::cout << "}" << std::endl;
				}
			}
		}
	public:
		virtual ~Parse() {}
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
		std::string& trim(std::string& str, char type = 0, std::string chars = " \t\n\r\f\v")
		{
			if (type == 0)
				return leftTrim(rightTrim(str, chars), chars);
			else if (type == 1)
				return rightTrim(str, chars);
			else
				return leftTrim(str, chars);
		}
};

#endif
