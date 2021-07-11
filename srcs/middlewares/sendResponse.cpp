/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 19:09:27 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:46:17 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serve/Serve.hpp"

void	sendResponse(Request &req, Response &res)
{
	std::stringstream	header;
	headers_t::iterator	it			= res.headers.begin();

	header << "HTTP/1.1 " << res.code << std::endl;
	while (it != res.headers.end())
	{
		header << *it << std::endl;
		++it;
	}
	header << std::endl;

	::send(res.fd, header.str().c_str(), header.str().length(), MSG_DONTWAIT);
	::send(res.fd, res.body.c_str(), res.body.length(), MSG_DONTWAIT | MSG_EOR);
	::close(res.fd);
	res.sent = true;
	res.logger.log(res.code, req.pathname);
}
