/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writeout.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 19:12:57 by badam             #+#    #+#             */
/*   Updated: 2022/03/10 22:52:10 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITEOUT_CPP
# define __WRITEOUT_CPP

bool	writeBuffer(Request &req, Response &res, std::string &str, unsigned long &size)
{
	int	ret;

	if (size > str.length())
		size = str.length();

	ret = ::send(res.fd, str.c_str(), size, MSG_DONTWAIT | MSG_NOSIGNAL);

	if (ret == -1)
	{
		req.await(EPOLLOUT);
		req.unfire(EPOLLOUT);
		res.logger.warn("Failed to write buffer right now");  // @TODO remove that log
		return (false);
	}
	else
	{
		str.erase(0, ret);
		size -= ret;
		return (true);
	}
}

#endif