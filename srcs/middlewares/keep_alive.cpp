#include "keep_alive.hpp"

bool	sendFinPacket(Request &req, Response &)
{
	if (req.closed() || !req.alive || !req.keep_alive)
		shutdown(req.fd, SHUT_WR);
	return (true);
}

bool	awaitClosed(Request &req, Response &)
{
	if (!req.alive || !req.keep_alive)
	{
		if (!(req.await(EPOLLHUP) || req.await(EPOLLRDHUP)) && !req.finish())
			return (false);
	}
		
	return (true);
}

bool	awaitNextRequest(Request &req, Response &res)
{
	std::string	line;

	if (req.closed() || !req.alive || !req.keep_alive)
		return (true);
	
	if (res.code != C_NOT_IMPLEMENTED)
	{
		req.reset();
		res.reset();

		// Take care: it may still anything to read through; not usable with EPOLLET
		if (req.buff.empty())
		{
			req.unfire(EPOLLIN);
			req.idle();
			return (false);
		}
	}

	if (req.timeout())
		return (true);
	
	if (!req.await(EPOLLIN))
		return (false);

	return (true);
}