#ifndef __MIDDLEWARE_GRACEFUL_SHUTDOWN_HPP
# define __MIDDLEWARE_GRACEFUL_SHUTDOWN_HPP
# include "webserv.hpp"

bool	sendFinPacket(Request &, Response &);
bool	awaitClosed(Request &, Response &);

#endif
