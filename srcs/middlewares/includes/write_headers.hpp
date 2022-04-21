#ifndef __MIDDLEWARE_WRITE_HEADERS_HPP
# define __MIDDLEWARE_WRITE_HEADERS_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"

bool	addResponseHeaders(Request &, Response &);
bool	serializeHeaders(Request &, Response &);
bool	sendHeader(Request &, Response &);

#endif
