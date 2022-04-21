#ifndef __MIDDLEWARE_REDIRECT_HPP
# define __MIDDLEWARE_REDIRECT_HPP
# include "webserv.hpp"

class Redirect: public IMiddleware
{
	typedef	IMiddleware	_parent;
	public:
		typedef struct	options_s
		{
			http_code_t	code;
			std::string	location;
		}				options_t;
		options_t		options;
		Redirect(http_code_t, std::string &);
		Redirect(options_t );
		virtual ~Redirect();
	public:
		bool	operator()(Request &, Response &);
};

#endif