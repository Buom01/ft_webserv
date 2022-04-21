#ifndef __MIDDLEWARE_FORBIDDEN_HPP
# define __MIDDLEWARE_FORBIDDEN_HPP
# include "webserv.hpp"

bool	forbidden(Request &, Response &);
bool	forbidden_method(Request &, Response &);

#endif
