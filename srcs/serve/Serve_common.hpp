/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve_common.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:42:27 by badam             #+#    #+#             */
/*   Updated: 2021/07/08 15:07:32 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_COMMON_HPP
# define SERVE_COMMON_HPP

# include <string>
# include <map>

/*
** From https://datatracker.ietf.org/doc/html/rfc7231#section-4
*/
typedef enum method_e
{
	M_UNKNOWN = 0,
	M_GET,
	M_HEAD,
	M_POST,
	M_PUT,
	M_DELETE,
	M_CONNECT,
	M_OPTIONS,
	M_TRACE,
	COUNT_M
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
	C_NOT_IMPLEMENTE = 501,
	C_BAD_GATEWAY = 502,
	C_SERVICE_UNAVAILABLE = 503,
	C_GATEWAY_TIMEOUT = 504,
	C_HTTP_VERSION_NOT_SUPPORTED = 505
} http_code_t;

typedef std::map<std::string, std::string>	headers_t;

#endif
