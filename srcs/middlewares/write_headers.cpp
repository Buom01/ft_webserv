/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_headers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/03/10 22:52:10 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_HEADERS_CPP
# define __WRITE_HEADERS_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"

# include "utils/writeout.cpp"


bool	addResponseHeaders(Request &, Response &res)
{
	Header				&h		= res.headers;
	intmax_t			length	= -1;


	h.set("Server: FT_WebServ");

	if (res.body.length() > 0)
		length = static_cast<intmax_t>(res.body.length());
	else if (res.response_fd)
		length = fdFileSize(res.response_fd);

	if (length >= 0)
	{
		std::stringstream	to_str;

		to_str << length;
		h.set("Content-Length: " + to_str.str());
	}
	
	if (length == 0 && res.code == C_OK)
		res.code = C_NO_CONTENT;  // @TODO: Verify this
	
	
	return (true);
}

bool	serializeHeaders(Request &req, Response &res)
{
	if (req.closed())
		return (true);

	std::stringstream			startline;
	std::vector<std::string>	ListHeaders = res.headers.VectorOfEveryHeaders();
	
	startline << "HTTP/1.1 " << res.code << "\n";
	res.headers_buff.append(startline.str());

	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
	{
		res.headers_buff.append(*it);
		res.headers_buff.push_back('\n');
	}
	res.headers_buff.push_back('\n');

	return (true);
}

bool	sendHeader(Request &req, Response &res)
{
	unsigned long	bufferSize;

	if (req.closed())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	while (res.headers_buff.length())
	{
		bufferSize = 1024;
		
		while(bufferSize)
		{
			if (!writeBuffer(req, res, res.headers_buff, bufferSize))
				return (false);
		}
	}
	
	res.headers_sent = true;
	res.logger.log(res.code, req.pathname);  // should it be there or at the end ??
	
	return (true);
}

#endif
