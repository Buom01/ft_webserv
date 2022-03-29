#ifndef __CGI
# define __CGI
# define ENV_SIZE 28
# define BUFFER_SIZE 1024
# define GATEWAY_VERSION "CGI/1.1"
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <string>
# include <map>
# include <vector>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "Split.hpp"
# include "MimeType.hpp"
# include "Header.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Url.hpp"

class cgiEnv
{
	protected:
		struct s_environment
		{
			std::string key;
			std::string value;
		} ENV[ENV_SIZE] =
		{
			{ "AUTH_TYPE", "" }, // Name of the authentication scheme (BASIC, SSL, or null)
			{ "CONTENT_LENGTH", "" }, // Length of the request body in bytes
			{ "CONTENT_TYPE", "" }, // MIME type of the body of the request, or null
			{ "GATEWAY_INTERFACE", "" }, // CGI specification. It is "CGI/1.1"

			{ "HTTP_ACCEPT", "" }, // Content types your browser supports
			{ "HTTP_ACCEPT_CHARSET", "" }, // Character preference information (utf-8;q=0.5)
			{ "HTTP_ACCEPT_ENCODING", "" }, // Defines type of encoding (compress;q=0.5)
			{ "HTTP_ACCEPT_LANGUAGE", "" }, // Languages you would prefer to receive content in (en;q=0.5)
			{ "HTTP_FORWARDED", "" }, // If the request was forwarded, shows address and port throw proxy server
			{ "HTTP_HOST", "" }, // Specifies Internet host and port number (Required for all HTTP/1.1 req)
			{ "HTTP_PROXY_AUTHORIZATION", "" }, // Used by a client to identify itself (or its user) to a proxy
			{ "HTTP_USER_AGENT", "" }, // Type and version of the browser of client (Mozilla/1.5)
			{ "HTTP_COOKIE", "" }, // HTTP Cookie String

			{ "PATH_INFO", "" }, // Identifies resource or sub-resource to be returned by CGI, derived from URI portion following the script name but preceding any query data
			{ "PATH_TRANSLATED", "" }, // Maps the script's virtual path to the physical path used to call the script
			{ "QUERY_STRING", "" }, // Query string that is contained in the request URL after the path
			{ "REMOTE_ADDR", "" }, // IP address of the client that sent the request
			{ "REMOTE_HOST", "" }, // Name of the client that sent the request, or the IP address
			{ "REMOTE_USER", "" }, // Login of the user making this request if the user has been authenticated, or null
			{ "REQUEST_METHOD", "" }, // Name of the HTTP method (GET, POST, ...)
			{ "SCRIPT_NAME", "" }, // Returns part of URL from the protocol name up to the query string in the first line of the HTTP request
			{ "SERVER_NAME", "" }, // Returns the host name of the server that received the request
			{ "SERVER_PORT", "" }, // Returns the port number on which this request was received
			{ "SERVER_PROTOCOL", "" }, // Returns the name and version of the protocol (HTTP/1.1)
			{ "SERVER_SOFTWARE", "" }, // Name and version of the server software is running
			{ "REQUEST_URI", "" }, // Interpreted pathname of the request document or CGI relative to root document
			{ "REDIRECT_STATUS", "" }, // HTML request code (200)
			{ "SCRIPT_FILENAME", "" } // Full pathname of current CGI
		};
	#pragma region one
		std::map<std::string, std::string>	env;
		char								**generateEnv;
	private:
		cgiEnv(cgiEnv const &rhs) {};
		cgiEnv &operator=(cgiEnv const &rhs) {};

		void clean()
		{
			for (int x = 0; generateEnv[x]; x++)
				delete generateEnv[x];
			delete [] generateEnv;
		}
	public:
		cgiEnv() {};
		~cgiEnv() { clean(); };

		/**
		 * Get environment variable
		 * @param key key of variable
		 * @return s_environment fill with variable, of empty if not exist
		 */
		s_environment	getVariable(std::string key)
		{
			for (int x = 0; x < ENV_SIZE; x++)
				if (ENV[x].key == key)
					return ENV[x];
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
			for (int x = 0; x < ENV_SIZE; x++)
			{
				if (ENV[x].key == key)
				{
					ENV[x].value.clear();
					ENV[x].value = value;
					return true;
				}
			}
			return false;
		}

		/**
		 * Delete environment variable
		 * @param key key of variable
		 * @return true if key found and delete
		 */
		bool			deleteVariable(std::string key)
		{
			for (int x = 0; x < ENV_SIZE; x++)
			{
				if (ENV[x].key == key)
				{
					ENV[x].value.clear();
					return true;
				}
			}
			return false;
		}

		/**
		 * Get correctly formated variables for cgi
		 * @return tab of environment
		 */
		char 			**envForCGI()
		{
			std::vector<s_environment>	temp;
			std::string					tempString;
			int x = 0;

			for (x = 0; x < ENV_SIZE; x++)
				if (!ENV[x].value.empty())
					temp.push_back(ENV[x]);
			clean();
			generateEnv = new char *[temp.size() + 1];
			x = 0;
			for (std::vector<s_environment>::iterator it = temp.begin(); it != temp.end(); it++)
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

class CGI : public cgiEnv
{
	private:
		cgiEnv	env;
	public:
		CGI() {};
		virtual ~CGI() {};
	private:
		CGI(CGI const &rhs) {};
		CGI &operator=(CGI const &rhs) {};
	#pragma endregion two
	private:
		char	*convertMethod(method_t method)
		{
			if (method == (1 << 0))
				return "GET";
			else if (method == (1 << 1))
				return "HEAD";
			else if (method == (1 << 2))
				return "POST";
			else if (method == (1 << 3))
				return "PUT";
			else if (method == (1 << 4))
				return "DELETE";
			else if (method == (1 << 5))
				return "CONNECT";
			else if (method == (1 << 6))
				return "OPTIONS";
			else if (method == (1 << 7))
				return "TRACE";
			else if (method == (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))
				return "ALL";
			else
				return "UNKNOWN";
		}

		void	setHeader(Request &req)
		{
			URL _url(req.pathname);
			Header::_Container headers = req.headers.GetEveryHeader();
			
			#pragma region Mandatory
				env.addVariable("CONTENT_LENGTH", headers.find("CONTENT_LENGTH")->second);
				env.addVariable("CONTENT_TYPE", headers.find("CONTENT_TYPE")->second);
				env.addVariable("GATEWAY_INTERFACE", GATEWAY_VERSION);
				env.addVariable("PATH_INFO", _url.pathname());
				env.addVariable("QUERY_STRING", _url.search());
				env.addVariable("REMOTE_ADDR", "127.0.0.1"); // IP of the agent sending the request to the server, need change
				env.addVariable("REQUEST_METHOD", convertMethod(req.method));
				env.addVariable("SCRIPT_NAME", _url.pathname());
				env.addVariable("SERVER_NAME", _url.host());
				env.addVariable("SERVER_PORT", _url.port());
				env.addVariable("SERVER_PROTOCOL", _url.protocol());
				env.addVariable("SERVER_SOFTWARE", SERVER_SOFTWARE);
			#pragma endregion Mandatory
			#pragma region Should
				env.addVariable("AUTH_TYPE", (_url.username()) ? "Basic" : NULL);
				env.addVariable("HTTP_ACCEPT", "");
				env.addVariable("HTTP_ACCEPT_CHARSET", "");
				env.addVariable("HTTP_ACCEPT_ENCODING", "");
				env.addVariable("HTTP_ACCEPT_LANGUAGE", "");
				env.addVariable("HTTP_COOKIE", "");
				env.addVariable("HTTP_FORWARDED", "");
				env.addVariable("HTTP_HOST", "");
				env.addVariable("HTTP_PROXY_AUTHORIZATION", "");
				env.addVariable("HTTP_USER_AGENT", "");
				env.addVariable("REMOTE_HOST", NULL);
			#pragma endregion Should
			#pragma region May
				env.addVariable("PATH_TRANSLATED", "");
				env.addVariable("REDIRECT_STATUS", 200);
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
			FILE		*tempIN = tmpfile(), *tempOUT = tmpfile();
			int			fdIN = fileno(tempIN), fdOUT = fileno(tempOUT);
			int			saveSTDIN = dup(STDIN_FILENO), saveSTDOUT = dup(STDOUT_FILENO);
			char		buffer[BUFFER_SIZE];
			pid_t		pid;
			
			setHeader(req);
			exit(-1);

			std::string body;

			write(fdIN, body.c_str(), std::atoi(getVariable("CONTENT_LENGTH").value.c_str()));
			lseek(fdIN, 0, SEEK_SET);
			if ((pid = fork()) == -1)
				return -1;
			if (pid == 0)
			{
				dup2(fdIN, STDIN_FILENO); dup2(fdOUT, STDOUT_FILENO);
				if (execve(getVariable("SCRIPT_FILENAME").value.c_str(), NULL, envForCGI()) == -1)
					write(STDOUT_FILENO, "Status: 500\r\n", 15);
			}
			else
			{
				waitpid(pid, NULL, 0);
				lseek(fdOUT, 0, SEEK_SET);
				/*
				memset(buffer, 0, sizeof(buffer));
				for (int ret = 1; ret > 0;)
				{
					ret = read(fdOUT, buffer, BUFFER_SIZE);
					buffer[ret] = 0;
					body += buffer;
					memset(buffer, 0, sizeof(buffer));
				}
				*/
			}
			dup2(saveSTDIN, STDIN_FILENO); dup2(saveSTDOUT, STDOUT_FILENO);
			fclose(tempIN); fclose(tempOUT);
			close(fdIN); close(fdOUT);
			close(saveSTDIN); close(saveSTDOUT);

			if (pid == 0)
				exit(EXIT_SUCCESS); // Child come if finished
			return fdOUT;
		}
};

bool cgi(Request &req, Response &res)
{
	CGI	instance;

	res.response_fd = instance.exec(req, res);
	return !!(res.response_fd != -1);
}

#endif
