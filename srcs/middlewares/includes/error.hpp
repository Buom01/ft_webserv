#ifndef __MIDDLEWARE_ERROR_HPP
# define __MIDDLEWARE_ERROR_HPP
# include "components.hpp"
# include "AEpoll.hpp"
# include "Log.hpp"

std::string	getErrorTitle(Response &);
std::string	getErrorMessage(Response &);
class Error: public AEpoll
{
	typedef	AEpoll	_parent;
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
		int	getErrorpageFD(int, Log &);
		std::string	generateErrorPage(Response &);
	public:
		bool	operator()(Request &, Response &);
};

#endif
