/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:42:44 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 19:22:54 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_HPP
# define SERVE_HPP
# include <sys/types.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <unistd.h>
# include <exception>
# include <sstream>
# include <string>
# include <vector>
# include "Header.hpp"
# include "Log.hpp"
# define SERVER_BUFFER_SIZE 2048
# define MAX_EVENTS 10
# define LISTEN_BACKLOG 10

class Request;
class Response;

typedef	struct sockaddr		sockaddr_t;
typedef	struct sockaddr_in	sockaddr_in_t;
typedef	struct epoll_event	epoll_event_t;

typedef struct server_bind_s
{
	int				fd;
	std::string		address;
	int				port;
	sockaddr_in_t	sockaddr_in;
	sockaddr_t		*sockaddr;
	socklen_t		len;
} server_bind_t;

typedef	std::vector<server_bind_t>	binds_t;

typedef	void (*middleware_t)(Request&, Response&);

typedef enum chain_flag_e
{
	F_NONE		= 0,
	F_NORMAL	= 1 << 0,
	F_ERROR		= 1 << 1,
	F_ALL		= F_NORMAL | F_ERROR
} chain_flag_t;

/*
** From https://datatracker.ietf.org/doc/html/rfc7231#section-4
*/
typedef enum method_e
{
	M_UNKNOWN	= 0,
	M_GET		= 1 << 0,
	M_HEAD		= 1 << 1,
	M_POST		= 1 << 2,
	M_PUT		= 1 << 3,
	M_DELETE	= 1 << 4,
	M_CONNECT	= 1 << 5,
	M_OPTIONS	= 1 << 6,
	M_TRACE		= 1 << 7,
	M_ALL		= M_GET | M_HEAD | M_POST | M_PUT | M_DELETE | M_CONNECT | M_OPTIONS | M_TRACE
} method_t;

/*
** From https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
*/
typedef enum http_code_e
{
	C_UNKNOWN = 0,

	C_CONTINUE = 100,
	C_SWITCHING_PROTOCOLS = 101,

	C_OK = 200,
	C_CREATED = 201,
	C_ACCEPTED = 202,
	C_NON_AUTHORITATIVE_INFORMATION = 203,
	C_NO_CONTENT = 204,
	C_RESET_CONTENT = 205,
	C_PARTIAL_CONTENT = 206,

	C_MULTIPLE_CHOICE = 300,
	C_MOVED_PERMANENTLY = 301,
	C_FOUND = 302,
	C_SEE_OTHER = 303,
	C_NOT_MODIFIED = 304,
	C_USE_PROXY = 305,
	C_TEMPORARY_REDIRECT = 307,

	C_BAD_REQUEST = 400,
	C_UNAUTHORIZED = 401,
	C_PAYMENT_REQUIRED = 402,
	C_FORBIDDEN = 403,
	C_NOT_FOUND = 404,
	C_METHOD_NOT_ALLOWED = 405,
	C_NOT_ACCEPTABLE = 406,
	C_PROXY_AUTHENTICATION_REQUIRED = 407,
	C_REQUEST_TIMEOUT = 408,
	C_CONFLICT = 409,
	C_GONE = 410,
	C_LENGTH_REQUIRED = 411,
	C_PRECONDITION_FAILED = 412,
	C_REQUEST_ENTITY_TOO_LARGE = 413,
	C_REQUEST_URI_TOO_LONG = 414,
	C_UNSUPPORTED_MEDIA_TYPE = 415,
	C_REQUEST_RANGE_NOT_SATISFIABLE = 416,
	C_EXPECTATION_FAILED = 417,

	C_INTERNAL_SERVER_ERROR = 500,
	C_NOT_IMPLEMENTED = 501,
	C_BAD_GATEWAY = 502,
	C_SERVICE_UNAVAILABLE = 503,
	C_GATEWAY_TIMEOUT = 504,
	C_HTTP_VERSION_NOT_SUPPORTED = 505
} http_code_t;

# include "Request.hpp"
# include "Response.hpp"

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
	binds_t			_binds;
	int				_poll_fd;
	chain_t			_response_chain;
	chain_t			_error_chain;

	public:
		Serve(void)
		{
		}
	
		virtual ~Serve(void)
		{
			binds_t::iterator	it		= _binds.begin();
			
			while (it != _binds.end())
			{
				try
				{
					close((*it).fd);
				}
				catch(...)
				{}

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
		server_bind_t	&_bindFromFD(int fd)
		{
			static server_bind_t	nullBind = {0};

			
			return (nullBind);
		}

	public:
		void	bind(std::string address, int port)  // Should close on error
		{
			server_bind_t		bind;
			int					opts	= 1;
			std::stringstream	error;

			if ((bind.fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1)
				throw new ServerSocketException("Socket creation failed");
			if (setsockopt(bind.fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) == -1)
				throw new ServerSocketException("Failed to set socket options");

			bind.address = address;
			bind.port = port;
			bind.sockaddr_in.sin_family = AF_INET;
			bind.sockaddr_in.sin_addr.s_addr = inet_addr(address.c_str());
			bind.sockaddr_in.sin_port = htons(port);
			bind.sockaddr = reinterpret_cast<sockaddr_t*>(&bind.sockaddr_in);
			bind.len = sizeof(bind.sockaddr_in);
			if (::bind(bind.fd, bind.sockaddr, bind.len) == -1)
			{
				error << "Fail to bind " << address << " to port " << port;
				_logger.fail(error.str(), errno);
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

			epoll_ev.events	= EPOLLIN | EPOLLOUT;
			
			while (it != _binds.end())
			{
				bind = *it;

				if (listen(bind.fd, 1) == -1)
					throw new ServerSocketException("Socket failed to listen");
			
				epoll_ev.data.fd	= bind.fd;
				if (epoll_ctl(_poll_fd, EPOLL_CTL_ADD, bind.fd, &epoll_ev) == -1)
					throw new ServerSocketException("EPOLL setup failed");
		
				_logger.greeting(bind.address, bind.port);

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

		void	exec(int connection, server_bind_t &bind)
		{
			Request 		req(connection, bind);
			Response		res(connection, _logger);

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

			return (ret);
		}

/*
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
*/

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
