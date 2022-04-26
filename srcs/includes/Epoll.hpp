#ifndef __EPOLL_HPP
# define __EPOLL_HPP
# include "builtin.hpp"
# include "lib.hpp"

typedef	std::vector<epoll_event_t>	events_t;

typedef enum	event_type_s
{
	ET_BIND = 0,
	ET_CONNECTION,
	ET_BODY,
	ET_FILE,
	ET_CGI
}				event_type_t;

typedef struct	event_data_s
{
	int				fd;
	event_type_t	type;
	void			*data;
}				event_data_t;


typedef struct	inner_event_s
{
	bool			fallback;
	epoll_event_t	*event;
}				inner_event_t;

typedef	std::map<int, inner_event_t>	epoll_events_map_t;

class	Epoll
{
	protected:
		Log					&_logger;
		int					_fd;
		epoll_events_map_t	_events;

	public:
		Epoll(Log &logger) :
			_logger(logger)
		{
			if ((_fd = epoll_create1(0)) == -1)
				throw EpollException("EPOLL creation failed");
		}

		~Epoll()
		{
			if (_fd >= 0)
				nothrow_close(_fd);
		}

		bool	has(int fd)
		{
			epoll_events_map_t::iterator	it	= _events.find(fd);

			return (it != _events.end());
		}

		void	add(int fd, event_type_t type, void *data, uint32_t events = EPOLLIN | EPOLLOUT)
		{
			epoll_event_t	*ev;
			event_data_t	*ev_data;
			inner_event_t	inner_ev;

			try
			{
				ev		= new epoll_event_t();
				ev_data	= new event_data_t();

				bzero(ev, sizeof(epoll_event_t));

				ev_data->fd		= fd;
				ev_data->type	= type;
				ev_data->data	= data;

				ev->events		= events | EPOLLRDHUP | EPOLLHUP;
				ev->data.ptr	= ev_data;

				inner_ev.fallback	= false;
				inner_ev.event		= ev;
				
				if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, ev) == -1)
				{
					if (errno == EPERM)
						inner_ev.fallback = true;
					else
						throw EpollException("EPOLL setup failed");
				}
				_events.insert(std::pair<int, inner_event_t>(fd, inner_ev));
			}
			catch(...)
			{
				if (ev && ev_data)
					epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL);
				if (ev)
				{
					delete ev;
					if (ev_data)
						delete ev_data;
				}

				throw EpollException("EPOLL setup failed");
			}
		}

		void	remove(int fd)
		{
			epoll_events_map_t::iterator	it;
			inner_event_t					inner_ev;
			event_data_t					*data;

			it = _events.find(fd);

			if (it == _events.end())
			{
				_logger.warn("Fail to find epoll event in the map");
				return ;
			}
			inner_ev = it->second;

			if (inner_ev.fallback || epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) == 0)
			{
				data = static_cast<event_data_t *>(inner_ev.event->data.ptr);

				_events.erase(it);
				delete inner_ev.event;
				delete data;
			}
			else
				_logger.warn("Failed to remove FD from epoll");
		}

		events_t	accept(void)
		{
			epoll_event_t					events[MAX_EVENTS];
			events_t						iterable_events;
			int								ret;
			epoll_events_map_t::iterator	it					= _events.begin();
			epoll_event_t					fake_event;

			ret = epoll_wait(_fd, events, MAX_EVENTS, 0);
			if (ret > 0)
				iterable_events.insert(iterable_events.begin(), events, events + ret);
			else if (ret < 0 && errno != EINTR)
				_logger.fail("Unexpected stop while waiting EPOLL");
			
			while (it != _events.end())
			{
				if (it->second.fallback)
				{
					fake_event = *(it->second.event);
					iterable_events.insert(iterable_events.end(), fake_event);
				}
				++it;
			}

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