#include "Chain.hpp"

void Chain::_log_error(RunningChain &instance, const std::exception &e)
{
	instance.res.logger.fail(e.what());
}

bool	Chain::_hasInterface(server_bind_t *interface, interfaces_t &interfaces)
{
	for (interfaces_t::const_iterator it = interfaces.begin(); it != interfaces.end(); ++it)
	{
		if (*it == interface)
			return (true);
	}
	return (false);
}

bool	Chain::_hasHostname(std::string &hostname, hostnames_t &hostnames, hostnames_t &onlyApplyOn)
{
	bool	isApplicable = false;

	for (hostnames_t::const_iterator it = onlyApplyOn.begin(); it != onlyApplyOn.end(); ++it)
	{
		if (*it == hostname)
		{
			isApplicable = true;
			break ;
		}
	}
	if (!isApplicable)
		return (true);

	for (hostnames_t::const_iterator it = hostnames.begin(); it != hostnames.end(); ++it)
	{
		if (*it == hostname)
			return (true);
	}
	return (false);
}

bool	Chain::_isSubpathOf(std::string &complete_path, std::string &base_path)
{
	if (base_path.length() > complete_path.length())
		return (false);
	return (complete_path.substr(0, base_path.length()) == base_path);
}

bool	Chain::_canUseLink(chain_link_t &link, Request &req, Response &res)
{
	if ( req.method != M_UNKNOWN && !(link.methods & req.method) )
		return (false);
	if (res.error && !(link.flag & F_ERROR))
		return (false);
	if ( req.pathname.length() && !_isSubpathOf(req.trusted_complete_pathname, link.pathname) )
		return (false);
	if (link.serverConfig.interfaces.size() && !_hasInterface(req.interface, link.serverConfig.interfaces))
		return (false);
	if (req.hostname.length() && link.serverConfig.hostnames.size())
	{
		if (!_hasHostname(req.hostname, link.serverConfig.hostnames, req.interface->hostnames))
			return (false);
	}
	return (true);
}

void	Chain::_root_at_locationblock(chain_link_t &link, Request &req)
{
	if (link.pathname.length() > 1 && _isSubpathOf(req.trusted_complete_pathname, link.pathname))
	{
		size_t	start_at = link.pathname.length();

		if (isDirectory(link.pathname) && start_at)
			--start_at;
		req.trusted_pathname = req.trusted_complete_pathname.substr(start_at, req.trusted_complete_pathname.length() - start_at);
	}
	else
		req.trusted_pathname = req.trusted_complete_pathname;
}

bool	Chain::_run(RunningChain &instance)
{
	chain_link_t		link;
	bool				ret;

	while (instance.pos != _raw_chain.end())
	{
		link = *(instance.pos);
		if (_canUseLink(link, instance.req, instance.res))
		{
			_root_at_locationblock(link, instance.req);
			if (link.middleware.obj)
				ret = (*link.middleware.obj)(instance.req, instance.res);
			else
				ret = (*link.middleware.fct)(instance.req, instance.res);
			if (!ret)
				return (false);
			else if (instance.req.finish())
				break ;
		}
		++(instance.pos);
	}
	if (instance.req.finish() && !instance.req.closed())
		instance.req.logger.warn("Connection was cut up cause of timeout");
	else if (!instance.res.sent && instance.res.code == C_OK)
		instance.res.logger.warn("Chain finished without sending data");
	return (true);
}

void	Chain::_handle_exception(RunningChain &instance, const std::exception &e)
{
	if (instance.res.error)
	{
		instance.res.logger.fail("Failed to send error to client");
		instance.res.sent = true;
		instance.pos = _raw_chain.end();
	}
	else
	{
		instance.state = CS_OTHER;
		_log_error(instance, e);
		instance.res.error = &e;
		instance.res.code = C_INTERNAL_SERVER_ERROR;
	}
}

bool	Chain::_exec_instance(RunningChain &instance)
{
	try
	{
		return _run(instance);
	}
	catch (const std::exception &e)
	{
		_handle_exception(instance, e);
		return (false);
	}
	catch (...)
	{
		instance.res.logger.fail("Unhandled exception");
			
		nothrow_close(instance.res.fd);
		return (true);
	}
}

Chain::Chain(Epoll &epoll): _epoll(epoll), _alive(true) {}

Chain::~Chain()
{
	running_chains_t::iterator	it	= _running.begin();

	while (it != _running.end())
	{
		unsafe_remove_instance(*it);
		++it;
	}
}

void Chain::unsafe_remove_instance(RunningChain *instance)
{
	running_chains_t::iterator	it	= _running.begin();

	while (it != _running.end())
	{
		if (*it == instance)
		{
			it = _running.erase(it);
			if (_epoll.has(instance->req.fd))
				_epoll.remove(instance->req.fd);
			break ;
		}
		else
			++it;
	}
	nothrow_close(instance->req.fd);
	delete instance;
}

void	Chain::use(IMiddleware &middleware, chain_flag_t flag, method_t methods, std::string &pathname, ServerConfig &serverConfig)
{
	chain_link_t	link;

	link.flag = flag;
	link.methods = methods;
	link.pathname = pathname;
	link.serverConfig = serverConfig;
	link.middleware.obj = &middleware;
	link.middleware.fct = NULL;
    _raw_chain.push_back(link);
}

void	Chain::use(bool (&middleware)(Request&, Response&), chain_flag_t flag, method_t methods, std::string &pathname, ServerConfig &serverConfig)
{
	chain_link_t	link;

	link.flag = flag;
	link.methods = methods;
	link.pathname = pathname;
	link.serverConfig = serverConfig;
	link.middleware.obj = NULL;
	link.middleware.fct = &middleware;
    _raw_chain.push_back(link);
}
		
RunningChain	*Chain::exec(int connection, server_bind_t *interface, std::string &client_ip, uint32_t events, Log &logger)
{
    RunningChain	*instance	= new RunningChain(connection, interface, client_ip, events, _alive, logger, _raw_chain.begin());

	if (!_exec_instance(*instance))
		_running.push_back(instance);
	else
	{
		unsafe_remove_instance(instance);
		instance = NULL;
	}
	return (instance);
}

bool	Chain::retake(RunningChain *instance, uint32_t events)
{
	if (instance->req.fire(events) && instance->state == CS_AWAIT_EVENT)
	{
		instance->state = CS_OTHER;
		if (_exec_instance(*instance))
		{
			unsafe_remove_instance(instance);
			return (true);
		}
	}
	return (false);
}

void	Chain::retake()
{
	running_chains_t::iterator	it	= _running.begin();

	while (it != _running.end())	
	{
		if (((*it)->state == CS_OTHER || (*it)->poll_timeout()) && _exec_instance(**it))
			unsafe_remove_instance(*it);
		else
			++it;
	}
}

bool	Chain::alive()
{
	return (_alive || _running.size());
}

void	Chain::stop()
{
	_alive = false;
}
