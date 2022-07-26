#ifndef __CGI_HPP
# define __CGI_HPP
# define ENV_NULL "NULL"
# define REDIRECT_STATUS "200"
# define SERVER_PROTOCOL "HTTP/1.1"
# define GATEWAY_VERSION "CGI/1.1"
# define PIPE_BUFFERSIZE 1024*1024*2
# include "builtin.hpp"
# include "lib.hpp"
# include "Log.hpp"
# include "AEpoll.hpp"
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
		void clean();
	public:
		cgiEnv();
		virtual ~cgiEnv();
	public:
		s_environment	getVariable(std::string);
		void			printVariable();
		bool			addVariable(std::string, std::string);
		bool			deleteVariable(std::string);
		char 			**envForCGI();
};

class CGI : public cgiEnv, public AEpoll
{
	typedef	AEpoll	_parent;

	private:
		cgiEnv			env;
		s_file 			file;
		Parse::s_cgi	_config;
		std::string		_location;
		std::string		_index;
		char 			**_argv;
	public:
		CGI(Parse::s_cgi, std::string, std::string, Log &);
		virtual ~CGI();
	private:
		std::string toLowerCase(std::string);
		bool		fileExtension(Request &);
		bool		isMethod(Request &);
		int 		stoi(std::string);
		std::string	itos(int);
		std::string convertMethod(method_t);
		std::string sval(std::string, std::string);
		std::string correctHeaderFormat(std::string);
		void		setHeader(Request &);
		bool		readHeaders(Request &, Response &);
		int			exec(Request &, Response &);
	public:
		bool 		operator()(Request &, Response &);
};

#endif
