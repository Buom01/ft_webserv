#ifndef __MIDDLEWARE_READ_HPP
# define __MIDDLEWARE_READ_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Url.hpp"

bool	parseStartLine(Request &, Response &);
bool	fulfillHostFromHeader(Request &, Response &);
bool	parseRequestHeaders(Request &, Response &);

#endif
