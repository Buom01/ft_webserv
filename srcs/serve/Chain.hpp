/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 16:19:54 by badam             #+#    #+#             */
/*   Updated: 2022/01/11 14:05:25 by badam            ###   ########.fr       */
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
		chain_t::iterator	pos;

		RunningChain(Request &_req, Response &_res, chain_t::iterator _pos):
			req(_req),
			res(_res),
			pos(_pos)
		{}
};

typedef std::vector<RunningChain *>    running_chains_t;


class   Chain
{
    protected:
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

			while (instance.pos != _raw_chain.end() && !instance.res.sent)
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
				}
				++(instance.pos);
			}
			if (!instance.res.sent)
				instance.res.logger->warn("Chain finished without sending data");
			return (true);
		}

		bool	_exec_instance(RunningChain &instance)
		{
			try
			{
				return _run(instance);
			}
			// catch (const std::exception &e)
			// {
			// 	instance.res.logger->fail(e.what());
			// 	instance.res.error = &e;

			// 	try
			// 	{
			// 		if (_error_chain)
			// 			_error_chain->exec(instance.req, instance.res);  // Should replace the running instance
			// 		else
			// 			throw (CantHandleRequest());
			// 	}
			// 	catch(const std::exception &ce)
			// 	{
			// 		instance.res.logger->fail("Failed to answer");

			// 		try
			// 		{
			// 			::close(instance.res.fd);
			// 		}
			// 		catch (...)
			// 		{}
			// 	}
			// }
			catch (...)
			{
				instance.res.logger->fail("Unhandled exception");

				return (true);
			}
		}
    
    public:
        Chain()
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
		
        void	exec(int connection, server_bind_t bind, Log &logger)
        {
			Request			req(connection, bind);
			Response		res(connection, logger);
            RunningChain	*instance	= new RunningChain(req, res, _raw_chain.begin());

			if (!_exec_instance(*instance))
				_running.push_back(instance);
        }
		
        void	exec(Request &req, Response &res)
        {
            RunningChain	*instance	= new RunningChain(req, res, _raw_chain.begin());

			if (!_exec_instance(*instance))
				_running.push_back(instance);
			else
				delete (instance);
        }

		void	retake()
		{
			running_chains_t::iterator	it	= _running.begin();

			std::cout << "Number of chain:" << _running.size() << std::endl;

			while (it != _running.end())
			{
				if (_exec_instance(**it))
				{
					delete *it;
					it = _running.erase(it);
				}
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