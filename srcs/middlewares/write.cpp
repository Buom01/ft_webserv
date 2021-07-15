/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2021/07/14 20:10:26 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_HEADERS_CPP
# define __WRITE_HEADERS_CPP
# include "Serve.hpp"

void	sendResponse(Request &req, Response &res)
{
	std::stringstream			header;
	std::vector<std::string>	ListHeaders = res.headers.VectorOfEveryHeaders();
	
	header << "HTTP/1.1 " << res.code << std::endl;
	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
		header << *it << std::endl;
	header << std::endl;
	::send(res.fd, header.str().c_str(), header.str().length(), MSG_DONTWAIT);
	::send(res.fd, res.body.c_str(), res.body.length(), MSG_DONTWAIT | MSG_EOR);
	::close(res.fd);
	res.sent = true;
	res.logger.log(res.code, req.pathname);
}

void	addResponseHeaders(Request &, Response &res)
{
	Header				&h		= res.headers;
	std::stringstream	to_str;

	h.set("Server: FT_WebServ");
	to_str << res.body.length();
	h.set("Content-Length: " + to_str.str());
	if (res.code == C_OK && res.body.length() == 0)
		res.code = C_NO_CONTENT;
}

#endif
