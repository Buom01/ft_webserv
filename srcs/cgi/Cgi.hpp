#ifndef __CGI_HPP
# define __CGI_HPP
# include <sys/stat.h>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstdlib>

# include <pthread.h>
# include <sys/wait.h>
# include <unistd.h>

# include "Url.hpp"
# include "Parse.hpp"
# include "Regex.hpp"
# include "Split.hpp"

struct CGI_Instance
{
	std::string location;
	std::string extension;
	std::string	path;
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
				else if (position == 2) instance.path = token;
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

		/**
		 * 	Update CGI if configuration file change
		 * 	@param Parse: reference to new Parse object 
		 */
		void updateCGI(const Parse& Parse)
		{
			_Parse = Parse;
			createCGIvector();
		}

		/**
		 *	Print CGI config for debug
		 */
		void printCGI()
		{
			for (std::vector<CGI_Instance>::iterator it = _CGI.begin(); it != _CGI.end(); ++it)
			{
				std::cout << it->location << " {" << std::endl;
				std::cout << "Extension:" << it->extension << "|" << std::endl;
				std::cout << "path:" << it->path << "|" << std::endl;
				std::cout << "GET:" << it->GET << "|" << std::endl;
				std::cout << "PUT:" << it->PUT << "|" << std::endl;
				std::cout << "POST:" << it->POST << "|" << std::endl;
				std::cout << "DELETE:" << it->DELETE << "|" << std::endl;
				std::cout << "}" << std::endl;
			}
		}
	
	// fork system part
	private:
		std::string getBlob(std::string filepath)
		{
			std::ifstream		in;
			std::ostringstream	sstr;
			in.open(filepath.c_str());
			sstr << in.rdbuf();
			return sstr.str();
		}

		int start(std::string filePath, std::string executablePath, char **envp)
		{
			pid_t		child;
			int			status;
			int			pipe_fd[2];
			std::string	raw = getBlob(filePath);

			if (pipe(pipe_fd) == -1)
			{
				std::cerr << "Pipe creation failed" << std::endl;
				return 1;
			}
			child = fork();
			if (child == -1)
			{
				std::cerr << "Fork creation failed" << std::endl;
				return 1;
			}
			else if (child == 0)
			{
				close(pipe_fd[0]);
				std::vector<char> blob(raw.begin(), raw.end()); blob.push_back('\0');
				char *argv[2];
				argv[0] = &blob[0];
				argv[1] = NULL;
				if (execve(executablePath.c_str(), argv, envp) == -1)
				{
					std::cerr << "Execve failed" << std::endl;
					close(pipe_fd[1]);
					exit(-1);
				}
				close(pipe_fd[1]);
				exit(0);
			}
			else
			{
				close(pipe_fd[1]);
				wait(&status);
				close(pipe_fd[0]);
			}
			return 0;
		}
	public:
		/**
		 * 	Check if cgi exist in this location. Execute script and return generate data. 
		 *  Return null if failed.
		 * 	@param location: complete url
		 */
		std::string execute(std::string location, char **envp)
		{
			URL			_url(location);
			Regex		regex;
			std::string	_path = "";
			std::string	_file_name;
			std::string	_file_extension;
			std::vector<std::string> directory = split(_url.pathname(), "/");
			size_t x = 0;
			for (std::vector<std::string>::iterator it = directory.begin(); it != directory.end(); ++it)
			{
				if (x < directory.size() - 1)
				{
					_path += *it;
					_path += "/";
				}
				else if (x < directory.size() && x >= directory.size() - 1)
					_file_name = *it;
				x++;
			}; _path.erase(_path.size() - 1);
			if (!_file_name.empty())
			{
				regex.Match(_file_name, "(.*)\\.(.*)");
				if (regex.GetSize() > 2)
				{
					_file_name = regex.GetMatch()[1].occurence;
					_file_extension = "." + regex.GetMatch()[2].occurence;
				}
				else
					_file_extension = "*";
			}
			for (std::vector<CGI_Instance>::iterator it = _CGI.begin(); it != _CGI.end(); ++it)
			{
				if (it->location == _path && it->extension == _file_extension)
				{
					std::cout << start(_url.pathname(), _path, envp) << std::endl;
				}
			}
			return "Nope";
		}
};

#endif
