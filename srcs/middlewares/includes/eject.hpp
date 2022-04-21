#ifndef __MIDDLEWARE_EJECT_HPP
# define __MIDDLEWARE_EJECT_HPP
# include "IMiddleware.hpp"

bool	eject(Request &, Response &);

class	EjectBody: public IMiddleware
{
	typedef IMiddleware	_parent;
	public:
		size_t	max_body_size;
		EjectBody(size_t);
		virtual ~EjectBody();
		bool	operator()(Request &, Response &);
};

#endif
