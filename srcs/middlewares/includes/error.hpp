#ifndef __MIDDLEWARE_ERROR_HPP
# define __MIDDLEWARE_ERROR_HPP
# include "webserv.hpp"

std::string	getErrorTitle(Response &);
std::string	getErrorMessage(Response &);
class Error: public AEpoll
{
	typedef	AEpoll	_parent;
	Log		&_logger;
	public:
		typedef	std::map<int, std::string>	errorpages_t;
		typedef struct	options_s
		{
			errorpages_t	errorpages;
		}				options_t;
		options_t		options;
		Error(Log &);
		Error(options_t, Log &);
		virtual ~Error();
		void	add(int, std::string);
		int	getErrorpageFD(int);
		std::string	generateErrorPage(Response &);
	public:
		bool	operator()(Request &, Response &);
};

#endif
