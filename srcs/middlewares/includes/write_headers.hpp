#ifndef __MIDDLEWARE_WRITE_HEADERS_HPP
# define __MIDDLEWARE_WRITE_HEADERS_HPP
# include "webserv.hpp"

bool	addResponseHeaders(Request &, Response &);
bool	serializeHeaders(Request &, Response &);
bool	sendHeader(Request &, Response &);

#endif
