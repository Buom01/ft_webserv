#ifndef __MIDDLEWARE_BODY_HPP
# define __MIDDLEWARE_BODY_HPP
# define LF "\n"
# define CRLF "\r\n"
# include "webserv.hpp"

bool	_get_contentlength(Request &);
bool	_get_transferEncoding(Request &, Response &);
bool	_get_bondary(Request &);
bool	body(Request &, Response &);

#endif
