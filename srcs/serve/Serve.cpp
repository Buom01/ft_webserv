/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 18:32:48 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:42:10 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../log/Log.hpp"
#include "Serve.hpp"


typedef struct server_link_s
{
	method_t		methods;
	std::string		pathname;
	middleware_t	middleware;
} server_link_t;

typedef std::vector<server_link_t>	chain_t;


class	Serve
{
	Log				_logger;
	int				_fd;
	addresses_t		_addresses;
	chain_t			_response_chain;
	chain_t			_error_chain;

	public:
		Serve(void)
		{
			if ((_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1)
				throw new ServerSocketException();
		}
	
		virtual ~Serve(void)
		{
			try
			{
				if (_fd >= 0)
					close(_fd);
			}
			catch(...)
			{}
		}

		void	bind(std::string address, int port)
		{
			server_address_t	addr;
			std::stringstream	error;

			bzero( &addr, sizeof(addr) );
			addr.address = address;
			addr.port = port;
			addr.sockaddr_in.sin_family = AF_INET;
			addr.sockaddr_in.sin_addr.s_addr = inet_addr(address.c_str());
			addr.sockaddr_in.sin_port = htons(port);
			addr.sockaddr = reinterpret_cast<sockaddr_t*>(&addr.sockaddr_in);
			addr.len = sizeof(addr.sockaddr_in);

			if (::bind(_fd, addr.sockaddr, addr.len) == -1)
			{
				error << "Fail to bind " << address << " to port " << port;
				_logger.fail(error.str(), errno);
			}
			else
				_addresses.push_back(addr);
		}

		void	begin(void)
		{
			addresses_t::iterator	it		= _addresses.begin();
			server_address_t		addr;

			if (listen(_fd, 1) == -1)
				throw new ServerSocketException("Socket failed to listen");
			
			while (it != _addresses.end())
			{
				addr = *it;
				_logger.greeting(addr.address, addr.port);
				++it;
			}
		}

		void	use(middleware_t middleware, chain_flag_t flag = F_NORMAL,
					method_t methods = M_ALL)
		{
			server_link_t	link;

			link.methods = methods;
			link.pathname = "";
			link.middleware = middleware;

			if (flag & F_NORMAL)
				_response_chain.push_back(link);
			if (flag & F_ERROR)
				_error_chain.push_back(link);
		}

		bool	canUseLink(server_link_t &link, Request &req)
		{
			return (
				( req.method == M_UNKNOWN  || (link.methods & req.method) )
				&& link.pathname.compare(req.pathname) <= 0
			);
		}

		void	execChain(chain_t &chain, Request &req, Response &res)
		{
			chain_t::iterator	it		= chain.begin();
			server_link_t		link;

			while (it != chain.end() && !res.sent)
			{
				link = *it;
				if (canUseLink(link, req))
					link.middleware(req, res);
				++it;
			}
			if (!res.sent)
				_logger.warn("Chain finished without sending data");
		}

		void	exec(int connection, server_address_t &addr)
		{
			Request 		req(connection, addr);
			Response		res(connection, addr, _logger);

			try
			{
				execChain(_response_chain, req, res);
			}
			catch (const std::exception &e)
			{
				_logger.fail(e.what());
				res.error = &e;

				try
				{
					execChain(_error_chain, req, res);
				}
				catch(const std::exception &ce)
				{
					_logger.fail("Failed to answer");

					try
					{
						::close(connection);
					}
					catch (...)
					{}
				}
			}
		}

		int		accept(void)
		{
			int						connection	= -1;
			addresses_t::iterator	it			= _addresses.begin();
			std::stringstream		error;
			server_address_t		addr;

			while (it != _addresses.end() && connection == -1)
			{
				addr = *it;
				connection = ::accept(_fd, addr.sockaddr, &addr.len);

				if (connection >= 0)
					exec(connection, addr);
				else if (errno != EWOULDBLOCK)
					_logger.fail("Fail to grab connection", errno);
				++it;
			}

			return (connection);
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
