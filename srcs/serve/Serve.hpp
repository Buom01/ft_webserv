/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:42:44 by badam             #+#    #+#             */
/*   Updated: 2022/03/18 07:25:07 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_HPP
# define SERVE_HPP
# include <exception>
# include <string>
# include <vector>
# include "http.hpp"
# include "IMiddleware.hpp"
# include "Log.hpp"
# include "Epoll.hpp"
# include "Chain.hpp"
# include "Request.hpp"
# include "Response.hpp"

class	Serve
{
	Epoll			_epoll;
	binds_t			_binds;
	Chain			_response_chain;
	bool			_alive;

	public:
		Log				logger;

		Serve(void):
			_epoll(logger),
			_response_chain(_epoll),
			_alive(false)
		{}
	
		virtual ~Serve(void)
		{
			binds_t::iterator	it		= _binds.begin();
			
			while (it != _binds.end())
			{
				_destroyBind(*it);
				++it;
			}
		}

	private:
		void			_destroyBind(server_bind_t &bind)
		{
			if (bind.fd)
			{
				_epoll.remove(bind.fd);
				nothrow_close(bind.fd);
			}
		}

		std::string		_netIpToStr(in_addr_t ip)
		{
			return (inet_ntoa(*reinterpret_cast<in_addr *>(&ip)));
		}

		in_addr_t		_ipFromHost(std::string host)
		{
			std::stringstream	error;
			in_addr_t			ip;
			struct hostent		*domain;

			if ((ip = inet_addr(host.c_str())) == INADDR_NONE)
			{
				if ((domain = gethostbyname(host.c_str())) == NULL)
				{
					error << "\"" << host << "\" is not recognized as a valid IP v4 address or know host";
					logger.fail(error.str());
					return (INADDR_NONE);
				}
				if (domain->h_addrtype != AF_INET)
				{
					error << "\"" << host << "\" is not IP v4";
					logger.fail(error.str());
					return (INADDR_NONE);
				}
				if ((ip = *(reinterpret_cast<in_addr_t *>(domain->h_addr))) == INADDR_NONE)
				{
					error << "\"" << host << "\" is can't be resolved as a valid IP v4 address";
					logger.fail(error.str());
					return (INADDR_NONE);
				}
			}

			return (ip);
		}

	public:
		void	bind(std::string host, uint16_t port)
		{
			in_addr_t			ip;
			server_bind_t		bind;
			int					opts	= 1;
			std::stringstream	error;

			if ((ip = _ipFromHost(host)) == INADDR_NONE)
				return ;
			if ((bind.fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1)
				throw ServerSocketException("Socket creation failed");
			if (setsockopt(bind.fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) == -1)
			{
				_destroyBind(bind);
				throw ServerSocketException("Failed to set socket options");
			}

			bind.host = host;
			bind.ip = _netIpToStr(ip);
			bind.port = port;
			bind.sockaddr_in.sin_family = AF_INET;
			bind.sockaddr_in.sin_addr.s_addr = ip;
			bind.sockaddr_in.sin_port = htons(port);
			bind.sockaddr = reinterpret_cast<sockaddr_t*>(&bind.sockaddr_in);
			bind.len = sizeof(bind.sockaddr_in);
			if (::bind(bind.fd, bind.sockaddr, bind.len) == -1)
			{
				error << "Fail to bind " << host << " [" << bind.ip << "] to port " << port;
				logger.fail(error.str(), errno);
				_destroyBind(bind);
			}
			else
				_binds.push_back(bind);
		}

		void	begin(void)
		{
			binds_t::iterator	it		= _binds.begin();
			server_bind_t		bind;

			while (it != _binds.end())
			{
				bind = *it;

				if (listen(bind.fd, 1) == -1)
					throw ServerSocketException("Socket failed to listen");
			
				_epoll.add(bind.fd, ET_BIND, NULL);

				logger.greeting(bind.host, bind.port);

				++it;
			}
			_alive = true;
		}

		void	use(IMiddleware &middleware, chain_flag_t flag = F_NORMAL, method_t methods = M_ALL, std::string pathname = "")
		{
			_response_chain.use(middleware, flag, methods, pathname);
		}

		void	use(bool (&middleware)(Request&, Response&), chain_flag_t flag = F_NORMAL, method_t methods = M_ALL, std::string pathname = "")
		{
			_response_chain.use(middleware, flag, methods, pathname);
		}

		RunningChain	*exec(int connection, uint32_t events)
		{
			return (_response_chain.exec(connection, events, logger));
		}

		bool	retake(RunningChain *instance, uint32_t events)
		{
			return (_response_chain.retake(instance, events));
		}

		void	retake()
		{
			_response_chain.retake();
		}

		void	accept(void)
		{
			events_t				events;
			events_t::iterator		it;
			int						connection;
			RunningChain			*chainInstance;
			event_data_t			data;

			retake();

			events = _epoll.accept();
			it = events.begin();

			while (it != events.end())
			{
				data = *static_cast<event_data_t *>(it->data.ptr);

				if (data.type == ET_CONNECTION)
				{
					retake(static_cast<RunningChain *>(data.data), it->events);
				}
				else if (data.type == ET_BIND)
				{
					connection	= ::accept(data.fd, NULL, NULL);

					if (connection >= 0)
					{
						if (_alive)
						{
							chainInstance = exec(connection, 0);
							
							if (chainInstance)
								_epoll.add(connection, ET_CONNECTION, chainInstance);
						}
						else
						{
							nothrow_close(connection);
							logger.warn("Reject connection");
						}
					}
					else
						logger.fail("Fail to grab connection", errno);
				}

				++it;
			}
		}

		bool	alive()
		{
			return (_alive || _response_chain.alive());
		}

		void	stop()
		{
			if (!_alive)
				return ;
			logger.stopping();
			_alive = false;
			_response_chain.stop();
		}

		class	ServerException: public std::runtime_error
		{
			public:
				ServerException(const std::string &msg = "Unknown internal server error.") :
					std::runtime_error(msg)
				{}
		};

		class	ServerSocketException: public ServerException
		{
			public:
				ServerSocketException(const std::string &msg = "Server socket exception.") :
					ServerException("ServerSocketException: " + msg)
				{}
		};
};

#endif
