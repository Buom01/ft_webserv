/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 16:19:54 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 07:08:05 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAIN_HPP
# define CHAIN_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "Log.hpp"

typedef struct	middleware_s
{
	IMiddleware	*obj;
	bool		(*fct)(Request&, Response&);
}				middleware_t;

typedef struct chain_link_s
{
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

		RunningChain(int connection, uint32_t _events, Log &logger, chain_t::iterator _pos):
			req(connection, _events, logger),
			res(connection, logger),
			events(_events),
			pos(_pos),
			state(CS_OTHER)
		{
			req.state = &state;
		}
};

typedef std::vector<RunningChain *>    running_chains_t;


class   Chain
{
    protected:
		Epoll				&_epoll;
        chain_t				_raw_chain;
		Chain				*_error_chain;
		running_chains_t	_running;

		bool	_canUseLink(chain_link_t &link, Request &req)
		{
			return (
				( req.method == M_UNKNOWN  || (link.methods & req.method) )
				&& link.pathname.compare(req.pathname) <= 0
			);
		}

		bool	_run(RunningChain &instance)
		{
			chain_link_t		link;
			bool				ret;

			while (instance.pos != _raw_chain.end())
			{
				link = *(instance.pos);
				if (_canUseLink(link, instance.req))
				{
					if (link.middleware.obj)
						ret = (*link.middleware.obj)(instance.req, instance.res);
					else
						ret = (*link.middleware.fct)(instance.req, instance.res);

					if (!ret)
						return (false);
					else if (instance.req.closed())
						break ;
				}
				++(instance.pos);
			}
			if (instance.req.closed())
				instance.res.logger.warn("Request closed by the client");
			else if (!instance.res.sent)
				instance.res.logger.warn("Chain finished without sending data");
			return (true);
		}

		bool	_exec_instance(RunningChain &instance)
		{
			// try
			// {
				return _run(instance);
			// }
			// catch (const std::exception &e)
			// {
			// 	instance.state = CS_OTHER;
			// 	instance.res.logger.fail(e.what());
			// 	instance.res.error = &e;

			// 	// try
			// 	// {
			// 	// 	if (_error_chain)
			// 	// 	{
			// 	// 		_error_chain->exec(instance.req, instance.res, instance.events);  // Should replace the running instance. Verify that's the case
			// 	// 		return (true);
			// 	// 	}
			// 	// 	else
			// 	// 		throw (CantHandleRequest());
			// 	// }
			// 	// catch(const std::exception &ce)
			// 	// {
			// 		instance.res.logger.fail("Failed to answer");

			// 		try
			// 		{
			// 			::close(instance.res.fd);
			// 		}
			// 		catch (...)
			// 		{}

			// 		return (true);
			// 	// }
			// }
			// catch (...)
			// {
			// 	instance.res.logger.fail("Unhandled exception");

			// 	try
			// 	{
			// 		::close(instance.res.fd);
			// 	}
			// 	catch(...)
			// 	{}

			// 	return (true);
			// }
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
			try
			{
				close(instance->req.fd);
			}
			catch(...)
			{}
			delete instance;
		}
    
    public:
        Chain(Epoll &epoll): _epoll(epoll)
        {}

        ~Chain()
        {
            // clearup  running
        }

		void	setErrorChain(Chain &error_chain)
		{
			_error_chain = &error_chain;
		}

		void	use(IMiddleware &middleware, method_t methods = M_ALL)
		{
			chain_link_t	link;

			link.methods = methods;
			link.pathname = "";
			link.middleware.obj = &middleware;
			link.middleware.fct = NULL;

            _raw_chain.push_back(link);
		}

		void	use(bool (&middleware)(Request&, Response&), method_t methods = M_ALL)
		{
			chain_link_t	link;

			link.methods = methods;
			link.pathname = "";
			link.middleware.obj = NULL;
			link.middleware.fct = &middleware;

            _raw_chain.push_back(link);
		}
		
        RunningChain	*exec(int connection, uint32_t events, Log &logger)
        {
            RunningChain	*instance	= new RunningChain(connection, events, logger, _raw_chain.begin());

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

		class CantHandleRequest : public std::exception
		{
			public:
				virtual const char* what() const throw () 
				{
					return ("No more fallback error chain");
				}
		};
};

#endif