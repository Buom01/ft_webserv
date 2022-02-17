/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 22:16:47 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 22:59:59 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <vector>
# include "http.hpp"
# include "Chain.hpp"

typedef	std::vector<epoll_event_t>	events_t;

typedef enum	event_type_s
{
	ET_BIND = 0,
	ET_CONNECTION,
	ET_FILE
}				event_type_t;

typedef struct	event_data_s
{
	int				fd;
	event_type_t	type;
	void			*data;
}				event_data_t;

typedef	std::map<int, epoll_event_t *>	epoll_events_map_t;

class	Epoll
{
	protected:
		Log					&_logger;
		int					_fd;
		epoll_events_map_t	_events;

	public:
		Epoll(Log &logger): _logger(logger)
		{
			if ((_fd = epoll_create1(0)) == -1)
				throw new EpollException("EPOLL creation failed");
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

		void	add(int fd, event_type_t type, void *data, uint32_t events = EPOLLIN | EPOLLOUT)
		{
			epoll_event_t	*ev			= new epoll_event_t();
			event_data_t	*ev_data	= new event_data_t();

			bzero(ev, sizeof(epoll_event_t));

			ev_data->fd		= fd;
			ev_data->type	= type;
			ev_data->data	= data;

			ev->events		= events;
			ev->data.ptr	= ev_data;

			_events.insert(std::pair<int, epoll_event_t*>(fd, ev));
			
			if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, ev) == -1)
				throw new EpollException("EPOLL setup failed");
		}

		void	remove(int fd)
		{
			epoll_events_map_t::iterator	it;
			epoll_event_t					*ev;
			event_data_t					*data;

			if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) == 0)
			{
				it = _events.find(fd);

				if (it == _events.end())
					_logger.warn("Fail to find epoll event in the map");
				else
				{
					ev = it->second;
					data = static_cast<event_data_t *>(ev->data.ptr);

					if (data->data)
					{
						// @TODO : Find a way to properly remove event's data
						
						// if (RunningChain *instance = dynamic_cast<RunningChain *>(data->data))
						// 	delete instance;
						// else
						// 	_logger.warn("Failed to remove epoll event's data");
					}

					delete ev;
					delete data;
				}
			}
			else
				_logger.warn("Failed to remove FD from epoll");
		}

		events_t	accept(void)
		{
			epoll_event_t	events[MAX_EVENTS];
			events_t		iterable_events;
			int				ret;

			ret = epoll_wait(_fd, events, MAX_EVENTS, 0);
			if (ret > 0)
				iterable_events.insert(iterable_events.begin(), events, events + ret);
			else if (ret < 0 && errno != EINTR)
				_logger.fail("Unexpected stop while waiting EPOLL");
				
			return (iterable_events);
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