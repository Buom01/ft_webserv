/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/01/10 19:36:28 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_HEADERS_CPP
# define __WRITE_HEADERS_CPP
# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"

bool	sendResponse(Request &req, Response &res)
{
	std::stringstream			header;
	std::vector<std::string>	ListHeaders = res.headers.VectorOfEveryHeaders();
	
	header << "HTTP/1.1 " << res.code << std::endl;
	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
		header << *it << std::endl;
	header << std::endl;
	::send(res.fd, header.str().c_str(), header.str().length(), MSG_DONTWAIT);
	::send(res.fd, res.body.str().c_str(), res.body.str().length(), MSG_DONTWAIT | MSG_EOR);
	::close(res.fd);
	res.sent = true;
	res.logger->log(res.code, req.pathname);
	
	return (true);
}

bool	addResponseHeaders(Request &, Response &res)
{
	Header				&h		= res.headers;
	std::stringstream	to_str;

	h.set("Server: FT_WebServ");
	to_str << res.body.str().length();
	h.set("Content-Length: " + to_str.str());
	if (res.code == C_OK && res.body.str().length() == 0)
		res.code = C_NO_CONTENT;
	
	return (true);
}

#endif
