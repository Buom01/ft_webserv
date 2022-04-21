#ifndef __MIDDLEWARE_REDIRECT_HPP
# define __MIDDLEWARE_REDIRECT_HPP
# include "webserv.hpp"

class Remover: public IMiddleware
{
	typedef	IMiddleware	_parent;
	public:
		typedef struct	options_s
		{
			std::string	destination;
			std::string	public_root;
		}				options_t;
		options_t		options;
		Remover(std::string, std::string);
		virtual ~Remover();
	public:
		bool	operator()(Request &, Response &);
};

#endif
