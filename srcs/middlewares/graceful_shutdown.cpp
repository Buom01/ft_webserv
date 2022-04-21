#include "graceful_shutdown.hpp"

bool	sendFinPacket(Request &req, Response &res)
{
	if (req.closed())
		res.logger.warn("Request closed by the client");
	shutdown(req.fd, SHUT_RDWR);
	return (true);
}

bool	awaitClosed(Request &req, Response &)
{
	if (req.timeout())
		return (true);
	if (!req.await(EPOLLHUP))
		return (false);
	return (true);
}
