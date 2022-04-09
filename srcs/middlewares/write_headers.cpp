/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_headers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/04/09 22:58:08 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_HEADERS_CPP
# define __WRITE_HEADERS_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "utils.hpp"

bool	addResponseHeaders(Request &, Response &res)
{
	Header		&h		= res.headers;
	off_t		length	= -1;

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
	if (req.finish())
		return (true);

	std::stringstream			startline;
	std::vector<std::string>	ListHeaders = res.headers.headers();
	
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
	ssize_t	send_ret = -1;
	size_t	write_size;

	if (req.finish())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	while (res.headers_buff.length())
	{
		write_size	= min(res.send_chunksize, res.headers_buff.length());
		send_ret = send(res.fd, res.headers_buff.c_str(), write_size, MSG_NOSIGNAL | MSG_DONTWAIT);

		if (send_ret > 0)
			res.headers_buff.erase(0, send_ret);
		if (send_ret < static_cast<ssize_t>(write_size))
		{
			req.unfire(EPOLLOUT);
			return (false);
		}
	}

	res.headers_sent = true;
	std::stringstream	infos;
	infos << get_elasped_ms(req.start);
	infos << "ms";
	res.logger.log(res.code, req.pathname, infos.str());
	
	return (true);
}

#endif
