/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 22:16:47 by badam             #+#    #+#             */
/*   Updated: 2022/01/26 23:56:17 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <vector>
# include "http.hpp"

typedef	std::vector<int>	epoll_fds_t;

class	Epoll
{
	protected:
		epoll_event_t	_epoll_ev;
		Log				_logger;
		int				_fd;


	public:
		Epoll(Log logger): _logger(logger)
		{
			if ((_fd = epoll_create1(0)) == -1)
				throw new EpollException("EPOLL creation failed");

			bzero(&_epoll_ev, sizeof(_epoll_ev));
			_epoll_ev.events	= EPOLLIN | EPOLLOUT;
		}

		~Epoll()
		{
			try
			{
				if (_fd >= 0)
					::close(_fd);
			}
			catch(...)
			{}
		}

		void	add(int fd)
		{
			_epoll_ev.data.fd	= fd;
			if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &_epoll_ev) == -1)
				throw new EpollException("EPOLL setup failed");
		}

		epoll_fds_t	accept(void)
		{
			epoll_event_t	events[MAX_EVENTS];
			int				ret;
			epoll_fds_t		fds;

			ret = epoll_wait(_fd, events, MAX_EVENTS, 1);
			if (ret > 0)
			{
				for (int i = 0; i < ret; ++i)
				{
					fds.push_back(events[i].data.fd);
				}
			}
			else if (ret < 0 && errno != EINTR)
				_logger.fail("Unexpected stop while waiting EPOLL");

			return (fds);
		}

		class	EpollException: public std::runtime_error
		{
			public:
				EpollException(std::string msg = "Unknown internal epoll error.") :
					std::runtime_error(msg)
				{}

				virtual ~EpollException() throw()
				{}
		};
};

#endif