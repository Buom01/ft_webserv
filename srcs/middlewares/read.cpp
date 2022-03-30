/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/03/30 04:16:07 by badam            ###   ########.fr       */
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

	regex.Match(line, "^([A-Z]+)\\ ([^\\ ]+)\\ HTTP\\/([0-9\\.]+)$");
	if (regex.GetSize() != 4)
		throw Serve::ServerSocketException("Malformed request start line");

	std::string &method = regex.GetMatch()[1].occurence;
	std::string &pathname = regex.GetMatch()[2].occurence;
	std::string &http_version = regex.GetMatch()[3].occurence;

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

		regex.Match(pathname, "^(\\/[^?]+)(\\?.*)?$");
		if (regex.GetSize() >= 3)
		{
			req.pathname = regex.GetMatch()[1].occurence;
			req.querystring = regex.GetMatch()[2].occurence;
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

	header_values = req.headers.GetHeader("host");
	header_values_it = header_values.begin();

	if (header_values_it != header_values.end())
	{
		std::string	host(*header_values_it);

		regex.Match(host, "^([^:]+)(:([0-9]{1,16}))?$");
		req.host = host;

		if (regex.GetSize() >= 2)
			req.hostname = regex.GetMatch()[1].occurence;
		if (regex.GetSize() >= 4)
			req.port = regex.GetMatch()[3].occurence;
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
