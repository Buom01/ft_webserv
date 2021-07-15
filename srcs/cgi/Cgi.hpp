#ifndef __CGI_HPP
# define __CGI_HPP
# include <sys/stat.h>
# include <vector>
# include <iostream>
# include "Parse.hpp"
# include "Regex.hpp"

struct CGI_Instance
{
	std::string location;
	std::string extension;
	std::string	directory;
	bool		GET;
	bool		PUT;
	bool		POST;
	bool		DELETE;
	CGI_Instance(void) : GET(false), PUT(false), POST(false), DELETE(false) {}
	~CGI_Instance() {}
};

class CGI
{
	private:
		Parse						_Parse;
		std::vector<s_server>		_Config;
		std::vector<CGI_Instance>	_CGI;
		std::string					save_location;
	private:
		void addCGI(std::pair<std::string, std::string> pair)
		{
			if (pair.first != "cgi_pass") return ;
			CGI_Instance	instance;
			instance.location = save_location;
			std::string str = pair.second;
			std::string token;
			char position = 1;
			size_t start;
			size_t end = 0;
			while ((start = str.find_first_not_of(" ", end)) != std::string::npos)
			{
				end = str.find(" ", start);
				token = str.substr(start, end - start);
				if (position == 1) instance.extension = token;
				else if (position == 2) instance.directory = token;
				else
				{
					if (token == "GET") instance.GET = true;
					if (token == "PUT") instance.PUT = true;
					if (token == "POST") instance.POST = true;
					if (token == "DELETE") instance.DELETE = true;
				}
				position++;
			}
			_CGI.push_back(instance);
			return;
		}

		void createCGIvector()
		{
			save_location.clear();
			_Config = _Parse.GetConfiguration();
			for (std::vector<s_server>::iterator it = _Config.begin(); it != _Config.end(); ++it)
			{
				for (std::vector<std::pair<std::string, std::string> >::iterator it2 = it->options.begin(); it2 != it->options.end(); it2++)
					addCGI(std::make_pair(it2->first, it2->second));
				for (std::vector<std::map<std::string, std::string> >::iterator it2 = it->locations.begin(); it2 != it->locations.end(); it2++)
				{
					std::string ret;
					std::map<std::string, std::string>::iterator find = it2->find("location");
					if (find != it2->end()) save_location = find->second;
					find = it2->find("cgi_pass");
					if (find != it2->end()) addCGI(std::make_pair(find->first, find->second));
				}
			}
		}
	public:
		CGI(const Parse& Parse) : _Parse(Parse) { createCGIvector(); }
		~CGI() {}

		void UpdateCGI(const Parse& Parse)
		{
			_Parse = Parse;
			createCGIvector();
		}

		void printCGI()
		{
			for (std::vector<CGI_Instance>::iterator it = _CGI.begin(); it != _CGI.end(); ++it)
			{
				std::cout << it->location << " {" << std::endl;
				std::cout << "Extension:" << it->extension << "|" << std::endl;
				std::cout << "Directory:" << it->directory << "|" << std::endl;
				std::cout << "GET:" << it->GET << "|" << std::endl;
				std::cout << "PUT:" << it->PUT << "|" << std::endl;
				std::cout << "POST:" << it->POST << "|" << std::endl;
				std::cout << "DELETE:" << it->DELETE << "|" << std::endl;
				std::cout << "}" << std::endl;
			}
		}
};

#endif
