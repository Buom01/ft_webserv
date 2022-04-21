#ifndef __MIDDLEWARE_GRACEFUL_SHUTDOWN_HPP
# define __MIDDLEWARE_GRACEFUL_SHUTDOWN_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"

bool	sendFinPacket(Request &, Response &);
bool	awaitClosed(Request &, Response &);

#endif
