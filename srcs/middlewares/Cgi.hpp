#ifndef __CGI
# define __CGI
# define BUFFER_SIZE 1024
# define GATEWAY_VERSION "CGI/1.1"
# define ENV_NULL "NULL"
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "Parse.hpp"
# include "Split.hpp"
# include "MimeType.hpp"
# include "Header.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Url.hpp"
# include "nullptr_t.hpp"
# include "http.hpp"

class cgiEnv
{
	protected:
		struct s_environment
		{
			std::string key;
			std::string value;
		};
	#pragma region one
		std::map<std::string, std::string>	env;
	public:
		std::vector<s_environment>			ENV;
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
		cgiEnv() { generateEnv = NULL; };
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

		/**
		 * Add environment variable
		 * @param key key of variable
		 * @param value value of variable
		 * @return true if key if correctly inserted
		 */
		bool			addVariable(std::string key, std::string value)
		{
			if (!value.empty())
				return false;
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
		cgiEnv	env;
		Parse::s_cgi _config;
	public:
		CGI(Parse::s_cgi config): _config(config) {};
		virtual ~CGI() {};
	#pragma endregion two
	private:
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

		void	setHeader(Request &req)
		{
			URL _url(req.trusted_pathname);
			#pragma region Mandatory
				env.addVariable("CONTENT_LENGTH", req.headers.header("CONTENT_LENGTH"));
				env.addVariable("CONTENT_TYPE", req.headers.header("CONTENT_TYPE"));
				env.addVariable("GATEWAY_INTERFACE", GATEWAY_VERSION);
				env.addVariable("PATH_INFO", req.pathname);
				env.addVariable("QUERY_STRING", req.querystring);
				env.addVariable("REMOTE_ADDR", req.client_ip);
				env.addVariable("REQUEST_METHOD", convertMethod(req.method));
				env.addVariable("SCRIPT_NAME", req.pathname);
				env.addVariable("SERVER_NAME", req.hostname);
				env.addVariable("SERVER_PORT", req.port);
				env.addVariable("SERVER_PROTOCOL", req.protocol);
				env.addVariable("SERVER_SOFTWARE", SERVER_SOFTWARE);
			#pragma endregion Mandatory
			#pragma region Should
				env.addVariable("AUTH_TYPE", (!_url.username().empty()) ? "Basic" : ENV_NULL);
				env.addVariable("HTTP_ACCEPT", req.headers.header("HTTP_ACCEPT"));
				env.addVariable("HTTP_ACCEPT_CHARSET", req.headers.header("HTTP_ACCEPT_CHARSET"));
				env.addVariable("HTTP_ACCEPT_ENCODING", req.headers.header("HTTP_ACCEPT_ENCODING"));
				env.addVariable("HTTP_ACCEPT_LANGUAGE", req.headers.header("HTTP_ACCEPT_LANGUAGE"));
				env.addVariable("HTTP_COOKIE", req.headers.header("HTTP_COOKIE"));
				env.addVariable("HTTP_FORWARDED", req.headers.header("HTTP_FORWARDED"));
				env.addVariable("HTTP_HOST", req.headers.header("HTTP_HOST"));
				env.addVariable("HTTP_PROXY_AUTHORIZATION", req.headers.header("HTTP_PROXY_AUTHORIZATION"));
				env.addVariable("HTTP_USER_AGENT", req.headers.header("HTTP_USER_AGENT"));
				env.addVariable("REMOTE_HOST", ENV_NULL);
			#pragma endregion Should
			#pragma region May
				env.addVariable("PATH_TRANSLATED", "");
				env.addVariable("REDIRECT_STATUS", "200");
				env.addVariable("REMOTE_USER", _url.username());
				env.addVariable("REMOTE_PASS", _url.password());
				env.addVariable("REQUEST_URI", "");
				std::vector<std::string> _split = split(_url.pathname(), "/");
				env.addVariable("SCRIPT_FILENAME", _split[_split.size() - 1]);
			#pragma endregion May
		}
	public:
		/**
		 * Execute cgi
		 * @return body generated by the cgi
		 */
		int	exec(Request &req, Response &res)
		{
			FILE	*OUT = tmpfile();
			int		fdOUT = fileno(OUT);
			int		saveSTDIN = dup(STDIN_FILENO), saveSTDOUT = dup(STDOUT_FILENO);
			int		fd[2];
			pid_t	pid;

			res.code = C_OK;
			if (pipe(fd))
				return -1;
			if ((pid = fork()) == -1)
				return -1;
			else if (pid == 0)
			{
				char* const* _null = NULL;

				dup2(fd[0], STDIN_FILENO);
				dup2(fdOUT, STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (execve(getVariable("SCRIPT_FILENAME").value.c_str(), _null, env.envForCGI()) == -1)
				{
					res.code = C_INTERNAL_SERVER_ERROR;
					std::cout << "Status: 500\r\n";
				}
			}
			else
			{
				write(fd[1], req.buff, std::strlen(req.buff));
				dup2(fd[1], req.fd);
				close(fd[0]);
				close(fd[1]);
				waitpid(pid, NULL, 0);
			}
			dup2(saveSTDIN, STDIN_FILENO); dup2(saveSTDOUT, STDOUT_FILENO);
			fclose(OUT);
			close(saveSTDIN); close(saveSTDOUT);
			if (pid == 0)
				exit(EXIT_SUCCESS);
			return fdOUT;
		}
	private:
		std::string 	fileExtension(Request &req)
		{
			std::string extension = req.pathname;
			extension = extension.substr(extension.find_last_of("."));
			return extension;
		}

		bool			isMethod(Request &req)
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
	public:
		bool operator()(Request &req, Response &res)
		{
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

			std::string extension = fileExtension(req);
			std::vector<std::string>::iterator it = std::find(_config.extensions.begin(), _config.extensions.end(), extension);
			if (it == _config.extensions.end() || !isMethod(req))
				return true;
			res.response_fd = exec(req, res);
			return true;
		}
};

#endif
