#ifndef __CGI_HPP
# define __CGI_HPP
# define ENV_NULL "NULL"
# define REDIRECT_STATUS "200"
# define SERVER_PROTOCOL "HTTP/1.1"
# define GATEWAY_VERSION "CGI/1.1"
# include "builtin.hpp"
# include "lib.hpp"
# include "IMiddleware.hpp"
# include "Request.hpp"
# include "Response.hpp"

class cgiEnv
{
	protected:
		struct s_environment
		{
			std::string key;
			std::string value;
		};
		struct s_file
		{
			std::string	path_info;	
			std::string	path;
			std::string	file;
			std::string	extension;
		};
		std::map<std::string, std::string>	env;
	public:
		std::vector<s_environment>			ENV;
		std::vector<std::string>			predefinedHeaders;
		char								**generateEnv;
	private:
		void clean()
		{
			if (generateEnv == NULL)
				return;
			for (int x = 0; generateEnv[x]; x++)
				delete [] generateEnv[x];
			delete [] generateEnv;
			generateEnv = NULL;
		}
	public:
		cgiEnv()
		{
			predefinedHeaders.push_back("auth-scheme");
			predefinedHeaders.push_back("content-length");
			predefinedHeaders.push_back("content-type");
			predefinedHeaders.push_back("remote-host");
			predefinedHeaders.push_back("transfer-encoding");
			generateEnv = NULL;
		};

		virtual ~cgiEnv() { clean(); };

		/**
		 * Get environment variable
		 * @param key key of variable
		 * @return s_environment fill with variable, of empty if not exist
		 */
		s_environment	getVariable(std::string key)
		{
			for (std::vector<s_environment>::const_iterator it = ENV.begin(); it != ENV.end(); it++)
			{
				if ((*it).key == key)
					return (*it);
			}
			return s_environment();
		}

		void	printVariable()
		{
			for (std::vector<s_environment>::const_iterator it = ENV.begin(); it != ENV.end(); it++)
				std::cout << (*it).key << "=" << (*it).value << std::endl;
		}

		/**
		 * Add environment variable
		 * @param key key of variable
		 * @param value value of variable
		 * @return true if key if correctly inserted
		 */
		bool			addVariable(std::string key, std::string value)
		{
			//if (value.empty())
			//	return false;
			s_environment temp;
			temp.key = key;
			temp.value = value;
			deleteVariable(key);
			ENV.push_back(temp);
			return true;
		}

		/**
		 * Delete environment variable
		 * @param key key of variable
		 * @return true if key found and delete
		 */
		bool			deleteVariable(std::string key)
		{
			for (std::vector<s_environment>::iterator it = ENV.begin(); it != ENV.end(); it++)
				if ((*it).key == key)
				{
					ENV.erase(it);
					return true;
				}
			return false;
		}

		/**
		 * Get correctly formated variables for cgi
		 * @return tab of environment
		 */
		char 			**envForCGI()
		{
			std::string	tempString;
			int			x = 0;

			clean();
			generateEnv = new char *[ENV.size() + 1];
			x = 0;
			for (std::vector<s_environment>::iterator it = ENV.begin(); it != ENV.end(); it++)
			{
				tempString = (*it).key + "=" + (*it).value;
				generateEnv[x] = new char[tempString.size() + 1];
				generateEnv[x] = std::strcpy(generateEnv[x], tempString.c_str());
				tempString.clear();
				++x;
			}
			generateEnv[x] = NULL;
			return generateEnv;
		}
};

class CGI : public cgiEnv, public IMiddleware
{
	private:
		cgiEnv			env;
		s_file 			file;
		Parse::s_cgi	_config;
		std::string		_location;
		std::string		_index;
	public:
		CGI(Parse::s_cgi config, std::string location, std::string index):
			_config(config),
			_location(location),
			_index(index)
		{};

		virtual ~CGI() {};
	private:
		std::string toLowerCase(std::string _string)
		{
			std::string ret = _string;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return ret;
		}

		void		fileExtension(Request &req)
		{
			size_t nposIndex(0);

			file.path_info = req.pathname;
			file.path = req.pathname;
			file.path.insert(0, ".");
			if (file.path == "./")
				file.path.append(_index);
			nposIndex = file.path.find_last_of("/");
			if (nposIndex == std::string::npos)
				return;
			file.file = file.path.substr(nposIndex + 1);
			file.extension = file.path.substr(file.path.find_last_of("."));
			file.path = concatPath(_config.root, file.path);
		}

		bool		isMethod(Request &req)
		{
			std::string method = convertMethod(req.method);
			if ((method == "GET" && _config.allow.GET == true) ||
				(method == "HEAD" && _config.allow.HEAD == true) ||
				(method == "POST" && _config.allow.POST == true) ||
				(method == "PUT" && _config.allow.PUT == true) ||
				(method == "DELETE" && _config.allow.DELETE == true) ||
				(method == "CONNECT" && _config.allow.CONNECT == true) ||
				(method == "OPTIONS" && _config.allow.OPTIONS == true) ||
				(method == "TRACE" && _config.allow.TRACE == true) ||
				(method == "ALL" && _config.allow.ALL == true))
				return true;
			return false;
		}

		int 		stoi(std::string number)
		{
			std::stringstream ss;
			int ret;

			ss << number;
			ss >> ret;
			return ret;
		}

		std::string	itos(int number)
		{
			std::stringstream ss;

			ss << number;
			return ss.str();
		}

		std::string convertMethod(method_t method)
		{
			if (method == 0)
				return "UNKNOWN";
			if (method == (1 << 0))
				return "GET";
			if (method == (1 << 1))
				return "HEAD";
			if (method == (1 << 2))
				return "POST";
			if (method == (1 << 3))
				return "PUT";
			if (method == (1 << 4))
				return "DELETE";
			if (method == (1 << 5))
				return "CONNECT";
			if (method == (1 << 6))
				return "OPTIONS";
			if (method == (1 << 7))
				return "TRACE";
			return "ALL";
		}

		std::string sval(std::string value, std::string _default)
		{
			if (value.empty())
				return _default;
			return value;
		}

		std::string correctHeaderFormat(std::string name)
		{
			if (name.empty())
				return "";
			for (std::string::iterator it = name.begin(); it != name.end(); it++)
			{
				if ((*it) == '-')
					(*it) = '_';
				else
					*it = std::toupper(*it);
			}
			name.insert(0, "HTTP_");
			return name;
		}

		void		setHeader(Request &req)
		{
			Header::_headers mapHeaders = req.headers.map();
			
			#pragma region Not defined by http headers
				env.addVariable("GATEWAY_INTERFACE", GATEWAY_VERSION);
				env.addVariable("PATH_INFO", file.path_info);
				env.addVariable("PATH_TRANSLATED", file.path_info);
				if (!req.querystring.empty() && req.querystring.at(0) == '?')
					req.querystring.erase(0, 1);
				env.addVariable("QUERY_STRING", req.querystring);
				env.addVariable("REDIRECT_STATUS", "200");
				env.addVariable("REMOTE_ADDR", req.client_ip);
				env.addVariable("REMOTE_IDENT", req.username);
				env.addVariable("REMOTE_PASS", req.password);
				env.addVariable("REMOTE_USER", req.username);
				env.addVariable("REQUEST_METHOD", convertMethod(req.method));
				env.addVariable("REQUEST_URI", file.path_info);
				env.addVariable("SCRIPT_FILENAME", file.path);
				env.addVariable("SCRIPT_NAME", file.file);
				env.addVariable("SERVER_NAME", req.hostname);
				env.addVariable("SERVER_PORT", req.port);
				env.addVariable("SERVER_PROTOCOL", SERVER_PROTOCOL);
				env.addVariable("SERVER_SOFTWARE", SERVER_SOFTWARE);
			#pragma endregion Not defined by http headers

			#pragma region Predefined Headers
				env.addVariable("AUTH_TYPE", sval(req.headers.header("AUTH-SCHEME", true), ((!req.username.empty()) ? "Basic" : "")));
				env.addVariable("CONTENT_LENGTH",
					sval(req.headers.header("CONTENT-LENGTH", true), itos(req.body.size()))
				);
				env.addVariable("CONTENT_TYPE", req.headers.header("CONTENT-TYPE", true));
				env.addVariable("REMOTE_HOST", sval(req.headers.header("REMOTE_HOST", true), ENV_NULL));
			#pragma endregion Predefined Headers

			#pragma region HTTP_*
			for (Header::_headers::iterator it = mapHeaders.begin(); it != mapHeaders.end(); it++)
			{
				std::string headerName(toLowerCase((*it).first)), var("");

				if (std::find(predefinedHeaders.begin(), predefinedHeaders.end(), headerName) == predefinedHeaders.end())
				{
					env.addVariable(
						correctHeaderFormat((*it).first),
						req.headers.header(headerName, true)
					);
				}
			}
			#pragma endregion HTTP_*
		}

		bool		setGenerateHeader(Request &, Response &res)
		{
			size_t npos(0);
			std::string	line, buff;
			
			while (get_next_line_string(res.response_fd, line, buff, res.logger))
			{
				if (line.empty())
				{
					npos += 2;
					break;
				}
				else
				{
					npos += line.size() + 2;
					res.headers.set(line);
				}
			}
			lseek(res.response_fd, npos, SEEK_SET);
			res.response_fd_header_size = npos;
			return (true);
		}
	public:
		/**
		 * Execute cgi
		 * @return body generated by the cgi
		 */
		int			exec(Request &req, Response &res)
		{
			FILE 	*IN = tmpfile(), *OUT = tmpfile();
			int 	fdIN = fileno(IN), fdOUT = fileno(OUT);
			int 	saveFd[2]; saveFd[0] = dup(STDIN_FILENO); saveFd[1] = dup(STDOUT_FILENO);
			pid_t	pid;

			setHeader(req);
			res.code = C_OK;
			write(fdIN, req.body.c_str(), static_cast<int>(req.body.size()));
			lseek(fdIN, 0, SEEK_SET);
			if ((pid = fork()) == -1)
				return -1;
			else if (pid == 0)
			{
				char * const * _null = NULL;

				dup2(fdIN, STDIN_FILENO);
				dup2(fdOUT, STDOUT_FILENO);
				if (execve(_config.path.c_str(), _null, env.envForCGI()))
				{
					res.code = C_INTERNAL_SERVER_ERROR;
					std::cout << "Status: 500\r\n";
				}
			}
			else
			{
				waitpid(pid, NULL, 0);
				lseek(fdOUT, 0, SEEK_SET);
			}
			dup2(saveFd[0], STDIN_FILENO);
			dup2(saveFd[1], STDOUT_FILENO);
			fclose(IN);
			close(fdIN);
			if (pid == 0)
			{
				fclose(OUT);
				close(fdOUT);
			}
			close(saveFd[0]); close(saveFd[1]);
			if (pid == 0)
				exit(EXIT_SUCCESS);
			return fdOUT;
		}

		bool 		operator()(Request &req, Response &res)
		{
			std::string empty("");

			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			if (req.finish())
				return (true);
			if (req.timeout())
			{
				res.code = C_REQUEST_TIMEOUT;
				return (true);
			}
			fileExtension(req);
			if (file.path == ".")
				return true;
			std::vector<std::string>::iterator it = std::find(_config.extensions.begin(), _config.extensions.end(), file.extension);
			if (it == _config.extensions.end() || !isMethod(req))
				return true;
			res.response_fd = exec(req, res);
			setGenerateHeader(req, res);
			return true;
		}
};

#endif
