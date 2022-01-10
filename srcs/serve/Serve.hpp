/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:42:44 by badam             #+#    #+#             */
/*   Updated: 2022/01/10 19:44:08 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_HPP
# define SERVE_HPP
# include <exception>
# include <string>
# include <vector>
# include "IMiddleware.hpp"
# include "Log.hpp"
# include "Chain.hpp"
# include "http.hpp"
# include "Request.hpp"
# include "Response.hpp"

class	Serve
{
	Log				_logger;
	binds_t			_binds;
	int				_poll_fd;
	Chain			_response_chain;
	Chain			_error_chain;

	public:
		Serve(void)
		{
			_response_chain.setErrorChain(_error_chain);
		}
	
		virtual ~Serve(void)
		{
			binds_t::iterator	it		= _binds.begin();
			
			while (it != _binds.end())
			{
				_destroyBind(*it);
				++it;
			}
			try
			{
				if (_poll_fd >= 0)
					::close(_poll_fd);
			}
			catch(...)
			{}
		}

	private:
		void			_destroyBind(server_bind_t &bind)
		{
			try
			{
				if (bind.fd)
					close(bind.fd);
			}
			catch(...)
			{}
		}

		std::string		_netIpToStr(in_addr_t ip)
		{
			return (inet_ntoa(*reinterpret_cast<in_addr *>(&ip)));
		}

		server_bind_t	&_bindFromFD(int fd)
		{
			static server_bind_t	nullBind = {0};
			binds_t::iterator		it		= _binds.begin();

			while (it != _binds.end())
			{
				if (it->fd == fd)
					return (*it);
			}
			
			_logger.warn("Bind not found for gived FD");
			return (nullBind);
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
					_logger.fail(error.str());
					return (INADDR_NONE);
				}
				if (domain->h_addrtype != AF_INET)
				{
					error << "\"" << host << "\" is not IP v4";
					_logger.fail(error.str());
					return (INADDR_NONE);
				}
				if ((ip = *(reinterpret_cast<in_addr_t *>(domain->h_addr))) == INADDR_NONE)
				{
					error << "\"" << host << "\" is can't be resolved as a valid IP v4 address";
					_logger.fail(error.str());
					return (INADDR_NONE);
				}
			}

			return (ip);
		}

	public:
		void	bind(std::string host, int port)
		{
			in_addr_t			ip;
			server_bind_t		bind;
			int					opts	= 1;
			std::stringstream	error;

			if ((ip = _ipFromHost(host)) == INADDR_NONE)
				return ;
			if ((bind.fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1)
				throw new ServerSocketException("Socket creation failed");
			if (setsockopt(bind.fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) == -1)
			{
				_destroyBind(bind);
				throw new ServerSocketException("Failed to set socket options");
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
				_logger.fail(error.str(), errno);
				_destroyBind(bind);
			}
			else
				_binds.push_back(bind);
		}

		void	begin(void)
		{
			epoll_event_t		epoll_ev;
			binds_t::iterator	it		= _binds.begin();
			server_bind_t		bind;

			if ((_poll_fd = epoll_create1(0)) == -1)
				throw new ServerSocketException("EPOLL creation failed");

			bzero(&epoll_ev, sizeof(epoll_ev));
			epoll_ev.events	= EPOLLIN | EPOLLOUT;
			
			while (it != _binds.end())
			{
				bind = *it;

				if (listen(bind.fd, 1) == -1)
					throw new ServerSocketException("Socket failed to listen");
			
				epoll_ev.data.fd	= bind.fd;
				if (epoll_ctl(_poll_fd, EPOLL_CTL_ADD, bind.fd, &epoll_ev) == -1)
					throw new ServerSocketException("EPOLL setup failed");
		
				_logger.greeting(bind.host, bind.port);

				++it;
			}
		}

		void	use(IMiddleware &middleware, chain_flag_t flag = F_NORMAL, method_t methods = M_ALL)
		{
			if (flag & F_NORMAL)
				_response_chain.use(middleware, methods);
			if (flag & F_ERROR)
				_error_chain.use(middleware, methods);
		}

		void	use(bool (&middleware)(Request&, Response&), chain_flag_t flag = F_NORMAL, method_t methods = M_ALL)
		{
			if (flag & F_NORMAL)
				_response_chain.use(middleware, methods);
			if (flag & F_ERROR)
				_error_chain.use(middleware, methods);
		}

		void	exec(int connection, server_bind_t &bind)
		{
			_response_chain.exec(connection, bind, _logger);
		}

		void	retake()
		{
			_error_chain.retake();
			_response_chain.retake();
		}

		void	accept(void)
		{
			epoll_event_t	events[MAX_EVENTS];
			int				ret;
			int				fd;
			server_bind_t	bind;
			int				connection;

			ret = epoll_wait(_poll_fd, events, MAX_EVENTS, 1);
			if (ret > 0)
			{
				for (int i = 0; i < ret; ++i)
				{
					fd = events[i].data.fd;
					connection	= ::accept(fd, NULL, NULL);

					if (connection >= 0)
					{
						bind = _bindFromFD(fd);
						exec(connection, bind);
					}
					else if (errno == EWOULDBLOCK)
						_logger.warn("EWOULDBLOCK happened with EPOLL");
					else
						_logger.fail("Fail to grab connection", errno);
				}
			}
			else if (ret < 0 && errno != EINTR)
				_logger.fail("Unexpected stop while waiting EPOLL");
		}

		class	ServerException: public std::runtime_error
		{
			public:
				ServerException(std::string msg = "Unknown internal server error.") :
					std::runtime_error(msg)
				{}

				virtual ~ServerException() throw()
				{}
		};

		class	ServerSocketException: public ServerException
		{
			public:
				ServerSocketException(std::string msg = "Server socket exception.") :
					ServerException(msg)
				{}
		};
};

#endif
