/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 16:19:54 by badam             #+#    #+#             */
/*   Updated: 2022/03/21 13:57:52 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAIN_HPP
# define CHAIN_HPP

# include "IMiddleware.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Log.hpp"
# include "File.hpp"

typedef struct	middleware_s
{
	IMiddleware	*obj;
	bool		(*fct)(Request&, Response&);
}				middleware_t;

typedef struct chain_link_s
{
	chain_flag_t	flag;
	method_t		methods;
	std::string		pathname;
	middleware_t	middleware;
} chain_link_t;

typedef std::vector<chain_link_t>	    chain_t;

class	RunningChain
{
	public:
		Request 			req;
		Response			res;
		uint32_t			events;
		chain_t::iterator	pos;
		chain_state_t		state;

		RunningChain(int connection, uint32_t _events, bool &_alive, Log &logger, chain_t::iterator _pos):
			req(connection, _events, _alive, logger),
			res(connection, logger),
			events(_events),
			pos(_pos),
			state(CS_OTHER)
		{
			req.state = &state;
		}

		virtual ~RunningChain()
		{
			if (res.response_fd > 0)
				nothrow_close(res.response_fd);
		}
};

typedef std::vector<RunningChain *>    running_chains_t;


class   Chain
{
    protected:
		Epoll				&_epoll;
        chain_t				_raw_chain;
		running_chains_t	_running;
		bool				_alive;

		void	_log_error(RunningChain &instance, const std::exception &e)
		{
			instance.res.logger.fail(e.what());
		}

		bool	_canUseLink(chain_link_t &link, Request &req, Response &res)
		{
			return (
				( req.method == M_UNKNOWN  || (link.methods & req.method) )
				&& link.pathname.compare(req.pathname) <= 0
				&& (!res.error || link.flag & F_ERROR)
			);
		}

		bool	_run(RunningChain &instance)
		{
			chain_link_t		link;
			bool				ret;

			while (instance.pos != _raw_chain.end())
			{
				link = *(instance.pos);
				if (_canUseLink(link, instance.req, instance.res))
				{
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
			if (instance.req.closed())
				instance.res.logger.warn("Request closed by the client");
			else if (instance.req.finish())
				instance.req.logger.warn("Connection was cut up cause of timeout");
			else if (!instance.res.sent && instance.res.code == C_OK)
				instance.res.logger.warn("Chain finished without sending data");
			return (true);
		}

		void	_handle_exception(RunningChain &instance, const std::exception &e)
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

		bool	_exec_instance(RunningChain &instance)
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

		void _remove_instance(RunningChain *instance)
		{
			running_chains_t::iterator	it	= _running.begin();

			while (it != _running.end())
			{
				if (*it == instance)
				{
					it = _running.erase(it);
					_epoll.remove(instance->req.fd);
					break ;
				}
				else
					++it;
			}

			nothrow_close(instance->req.fd);
			delete instance;
		}
    
    public:
        Chain(Epoll &epoll):
			_epoll(epoll),
			_alive(true)
        {}

        ~Chain()
        {
			running_chains_t::iterator	it	= _running.begin();

			while (it != _running.end())
			{
				_remove_instance(*it);
				++it;
			}
		}

		void	use(IMiddleware &middleware, chain_flag_t flag, method_t methods, std::string &pathname)
		{
			chain_link_t	link;

			link.flag = flag;
			link.methods = methods;
			link.pathname = "";
			link.middleware.obj = &middleware;
			link.middleware.fct = NULL;

            _raw_chain.push_back(link);
		}

		void	use(bool (&middleware)(Request&, Response&), chain_flag_t flag, method_t methods, std::string &pathname)
		{
			chain_link_t	link;

			link.flag = flag;
			link.methods = methods;
			link.pathname = "";
			link.middleware.obj = NULL;
			link.middleware.fct = &middleware;

            _raw_chain.push_back(link);
		}
		
        RunningChain	*exec(int connection, uint32_t events, Log &logger)
        {
            RunningChain	*instance	= new RunningChain(connection, events, _alive, logger, _raw_chain.begin());

			if (!_exec_instance(*instance))
				_running.push_back(instance);
			else
			{
				_remove_instance(instance);
				instance = NULL;
			}

			return (instance);
        }

		bool	retake(RunningChain *instance, uint32_t events)
		{
			if (instance->req.fire(events) && instance->state == CS_AWAIT_EVENT)
			{
				instance->state = CS_OTHER;
				
				if (_exec_instance(*instance))
				{
					_remove_instance(instance);
					return (true);
				}
			}
			
			return (false);
		}

		void	retake()
		{
			running_chains_t::iterator	it	= _running.begin();

			while (it != _running.end())
			{
				if ((*it)->state == CS_OTHER && _exec_instance(**it))
					_remove_instance(*it);
				else
					++it;
			}
		}

		bool	alive()
		{
			return (_alive || _running.size());
		}

		void	stop()
		{
			_alive = false;
		}
};

#endif