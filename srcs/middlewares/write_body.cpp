/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_body.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/04/12 03:39:52 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_BODY_CPP
# define __WRITE_BODY_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "AEpoll.hpp"
# include "utils.hpp"
# include "File.hpp"

class SendBodyFromFD: public AEpoll
{
	typedef	AEpoll	_parent;

	public:
		SendBodyFromFD(Log &logger): _parent(logger)
		{}

		virtual ~SendBodyFromFD()
		{}
		
	public:
		bool	operator()(Request &req, Response &res)
		{
			ssize_t	read_ret = -1;
			ssize_t	send_ret = -1;
			char	read_buffer[res.send_chunksize];
			
			if (res.sent)
				return (true);
			if (!res.response_fd)
				return (true);
			else if (req.method == M_HEAD)
			{
				nothrow_close(res.response_fd);
				res.response_fd = 0;
				return (true);
			}
			if (req.finish())
			{
				if (_parent::has(res.response_fd))
					_parent::cleanup(res.response_fd);
				return (true);
			}
				
			if (!req.await(EPOLLOUT))
				return (false);
			if (!_parent::has(res.response_fd))
				_parent::setup(res.response_fd, ET_BODY);
			if (!_parent::await(res.response_fd, EPOLLIN))
				return (false);
			
			while (read_ret != 0 || send_ret != 0)
			{
				send_ret = send(res.fd, res.response_fd_buff.c_str(), res.response_fd_buff.length(), MSG_NOSIGNAL | MSG_DONTWAIT);
				if (send_ret > 0)
					res.response_fd_buff.erase(0, send_ret);
				if (res.response_fd_buff.length())
				{
					req.unfire(EPOLLOUT);
					return (false);
				}

				read_ret = read(res.response_fd, read_buffer, res.send_chunksize);
				if (read_ret == -1)
				{
					_parent::clear_events(res.response_fd, EPOLLIN);
					return (false);
				}
				res.response_fd_buff.append(read_buffer, read_ret);
			}

			res.sent = true;
			_parent::cleanup(res.response_fd);
			nothrow_close(res.response_fd);
			res.response_fd = 0;
			
			return (true);
		}

};

bool	sendBodyFromBuffer(Request &req, Response &res)
{
	ssize_t	send_ret = -1;
	size_t	write_size;

	if (res.sent)
		return (true);
	if (!res.body.length())
		return (true);
	else if (req.method == M_HEAD)
	{
		res.sent = true;
		return (true);
	}
	if (req.finish())
		return (true);

	if (!req.await(EPOLLOUT))
		return (false);

	while (res.body.length())
	{
		write_size	= min(res.send_chunksize, res.body.length());
		send_ret = send(res.fd, res.body.c_str(), write_size, MSG_NOSIGNAL | MSG_DONTWAIT);
		
		if (send_ret > 0)
			res.body.erase(0, send_ret);
		if (send_ret < static_cast<ssize_t>(write_size))
		{
			req.unfire(EPOLLOUT);
			return (false);
		}
	}
	
	res.sent = true;

	return (true);
}

#endif
