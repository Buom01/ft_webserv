#include "Definition.hpp"

ServerConfig::ServerConfig(): hostnames(), interfaces() {};

RunningChain::RunningChain(
	int connection,
	server_bind_t *interface,
	std::string &client_ip,
	uint32_t _events,
	bool &_alive,
	Log &logger,
	chain_t::iterator _pos
):
	req(connection, interface, client_ip, _events, _alive, logger),
	res(connection, logger),
	events(_events),
	pos(_pos),
	state(CS_WORKING),
	wait_timeout(false)
{
	req.state			= &state;
	req.wait_since		= &wait_since;
	req.wait_timeout	= &wait_timeout;
}

RunningChain::~RunningChain()
{
	if (res.response_fd > 0)
		nothrow_close(res.response_fd);
}

bool RunningChain::poll_timeout()
{
	if (wait_timeout || get_elasped_ns(wait_since) >= (int64_t)30 * 1000000000)
	{
		wait_timeout = true;
		return (true);
	}
	else
		return (false);
}
