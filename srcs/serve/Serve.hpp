/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:42:44 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:36:59 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_HPP
# define SERVE_HPP

# include <exception>
# include <sstream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <unistd.h>
# include <vector>

# define SERVER_BUFFER_SIZE 100


class Request;
class Response;

typedef	struct sockaddr		sockaddr_t;
typedef	struct sockaddr_in	sockaddr_in_t;

typedef struct server_address_s
{
	std::string		address;
	int				port;
	sockaddr_in_t	sockaddr_in;
	sockaddr_t		*sockaddr;
	socklen_t		len;
} server_address_t;

typedef	std::vector<server_address_t>	addresses_t;

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
	M_ALL		= M_GET | M_HEAD | M_POST | M_PUT | M_DELETE | M_CONNECT |
					M_OPTIONS | M_TRACE
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

# include "../middlewares/header.cpp"
# include "Request.hpp"
# include "Response.hpp"

# include "Serve.cpp"

#endif
