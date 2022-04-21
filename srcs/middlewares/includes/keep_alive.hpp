#ifndef __MIDDLEWARE_KEEP_ALIVE_HPP
# define __MIDDLEWARE_KEEP_ALIVE_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"

bool	sendFinPacket(Request &, Response &);
bool	awaitClosed(Request &, Response &);
bool	awaitNextRequest(Request &, Response &);

#endif
