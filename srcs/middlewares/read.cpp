/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2021/08/17 17:49:41 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __READ_HEADERS_CPP
# define __READ_HEADERS_CPP
# include <string>
# include "GNL.hpp"
# include "Serve.hpp"
# include "Regex.hpp"
# include "File.hpp"

	void	parseStartLine(Request &req, Response &)
	{
		std::string	line;
		Regex		regex;

		if (get_next_line_string(req.fd, line, req.buff, SERVER_BUFFER_SIZE) < 0)
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
		req.trusted_pathname = sanitizeRelativePath(req.pathname);
		req.http_version = http_version;

		if (http_version != "1.0" && http_version != "1.1")
			throw new Serve::ServerSocketException("Unsupported HTTP version");
		if (req.method == M_UNKNOWN)
			throw new Serve::ServerSocketException("Unsupported method");
	}
	
	void	parseRequestHeaders(Request &req, Response &)
	{
		std::string	line;

		while (get_next_line_string(req.fd, line, req.buff, SERVER_BUFFER_SIZE) > 0 && line.length())
			req.headers.set(line);
	}

#endif
