/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseStartLine.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 14:32:57 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:46:34 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serve/Serve.hpp"
#include "../parse/get_next_line/get_next_line.hpp"
#include "../parse/regex.hpp"

/*
** It parse the request's start line, getting the method, the pathname,
** and finally the HTTP version
*/
void	parseStartLine(Request &req, Response &)
{
	std::string	line;
	Regex		regex;

	if (get_next_linestring(req.fd, line, req.buff, SERVER_BUFFER_SIZE) < 0)
		throw new Serve::ServerSocketException("Failed to read start line");

	regex.Match(line, "^([A-Z]+)\\ ([^\\ ]+)\\ HTTP\\/([0-9\\.]+)$");
	if (regex.GetSize() != 4)
		throw new Serve::ServerSocketException("Malformed request start line");

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
	req.http_version = http_version;

	if (http_version != "1.0" && http_version != "1.1")
		throw new Serve::ServerSocketException("Unsupported HTTP version");
	if (req.method == M_UNKNOWN)
		throw new Serve::ServerSocketException("Unsupported method");
}
