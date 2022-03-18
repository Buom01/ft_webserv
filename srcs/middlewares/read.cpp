/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/03/18 07:48:59 by badam            ###   ########.fr       */
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
# include "File.hpp"

/*
** @todo: retake should not enter into these chain part if there are no incomming event in epoll
** alternative should be to change a variable in request if new epoll event happpened
*/
bool	parseStartLine(Request &req, Response &)
{
	std::string	line;
	Regex		regex;

	if (req.finish())
		return (true);
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

	req.pathname = pathname;
	req.trusted_pathname = sanitizeRelativePath(req.pathname);
	req.http_version = http_version;

	if (http_version != "1.0" && http_version != "1.1")
		throw Serve::ServerSocketException("Unsupported HTTP version");
	if (req.method == M_UNKNOWN)
		throw Serve::ServerSocketException("Unsupported method");

	return (true);
}

bool	parseRequestHeaders(Request &req, Response &)
{
	std::string	line;

	if (req.finish())
		return (true);
	if (!req.await(EPOLLIN))
		return (false);

	while (get_next_line_string(req.fd, line, req.buff, SERVER_BUFFER_SIZE) > 0 && line.length())
	{
		if (line.length())
			return (true);
		else
			req.headers.set(line);
	}

	return (false);
}

#endif
