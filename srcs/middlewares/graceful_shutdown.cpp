/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graceful_shutdown.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/04/20 19:33:12 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __GRACEFUL_SHUTDOWN_CPP
# define __GRACEFUL_SHUTDOWN_CPP
# include "Request.hpp"
# include "Response.hpp"

bool	sendFinPacket(Request &req, Response &res)
{
	if (req.closed())
		res.logger.warn("Request closed by the client");
	shutdown(req.fd, SHUT_RDWR);
	return (true);
}

bool	awaitClosed(Request &req, Response &)
{
	if (req.timeout())
		return (true);
	if (!req.await(EPOLLHUP))
		return (false);
		
	return (true);
}

#endif
