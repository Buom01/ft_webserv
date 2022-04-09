/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/04/03 21:40:42 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __READ_HEADERS_CPP
# define __READ_HEADERS_CPP
# include <string>
# include "GNL.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "Regex.hpp"
# include "Url.hpp"
# include "File.hpp"

bool	parseStartLine(Request &req, Response &res)
{
	std::string	line;
	Regex		regex;

	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}
	if (!req.await(EPOLLIN))
		return (false);

	if (get_next_line_string(req.fd, line, req.buff, SERVER_BUFFER_SIZE) < 0)
		return (false);

	regex.exec(line, "^([A-Z]+)\\ ([^\\ ]+)\\ HTTP\\/([0-9\\.]+)$", GLOBAL_FLAG);
	if (regex.size() != 3)
		throw Serve::ServerSocketException("Malformed request start line");

	std::string &method = regex.match()[0].occurence;
	std::string &pathname = regex.match()[1].occurence;
	std::string &http_version = regex.match()[2].occurence;

	if (method == "GET")
		req.method = M_GET;
	else if (method == "HEAD")
		req.method = M_HEAD;
	else if (method == "POST")
		req.method = M_POST;
	else if (method == "PUT")
		req.method = M_PUT;
	else if (method == "DELETE")
		req.method = M_DELETE;
	else if (method == "CONNECT")
		req.method = M_CONNECT;
	else if (method == "OPTIONS")
		req.method = M_OPTIONS;
	else if (method == "TRACE") 
		req.method = M_TRACE;

	req.method_str = method;
	req.raw_pathname = pathname;
	req.http_version = http_version;

	if (pathname[0] == '/' || pathname[0] == '*')
	{
		Regex   regex;

		regex.exec(pathname, "^(\\/[^?]+)(\\?.*)?$");
		if (regex.size() >= 2)
		{
			req.pathname = regex.match()[0].occurence;
			req.querystring = regex.match()[1].occurence;
		}
		else
			req.pathname = pathname;
		req.trusted_pathname = sanitizeRelativePath(req.pathname);
	}
	else
	{
		URL	uri(pathname);

		req.protocol = uri.protocol();
		req.host = uri.host();
		req.hostname = uri.hostname();
		req.port = uri.port();
		req.pathname = uri.pathname();
		req.querystring = uri.search();
		req.trusted_pathname = sanitizeRelativePath(req.pathname);
	}

	return (true);
}

bool	fulfillHostFromHeader(Request &req, Response &)
{
	Regex   									regex;
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	header_values = req.headers.headerValues("host");
	header_values_it = header_values.begin();

	if (header_values_it != header_values.end())
	{
		std::string	host(*header_values_it);

		regex.exec(host, "^([^:]+)(:([0-9]{1,16}))?$", GLOBAL_FLAG);
		req.host = host;

		if (regex.size() >= 1)
			req.hostname = regex.match()[0].occurence;
		if (regex.size() >= 3)
			req.port = regex.match()[2].occurence;
	}

	return (true);
}

bool	parseRequestHeaders(Request &req, Response &res)
{
	std::string	line;

	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}
	if (!req.await(EPOLLIN))
		return (false);

	while (get_next_line_string(req.fd, line, req.buff, SERVER_BUFFER_SIZE) > 0)
	{
		if (!line.length())
		{
			fulfillHostFromHeader(req, res);
			return (true);
		}
		else
			req.headers.set(line);
	}

	return (false);
}

#endif
