#ifndef __MIDDLEWARE_READ_HPP
# define __MIDDLEWARE_READ_HPP
# include "webserv.hpp"

bool	parseStartLine(Request &, Response &);
bool	fulfillHostFromHeader(Request &, Response &);
bool	parseRequestHeaders(Request &, Response &);

#endif
