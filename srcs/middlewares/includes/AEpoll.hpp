#ifndef __AEPOLL_HPP
# define __AEPOLL_HPP
# include "builtin.hpp"
# include "lib.hpp"
# include "IMiddleware.hpp"
# include "Epoll.hpp"
# include "Log.hpp"

class AEpoll: public IMiddleware
{
	private:
		Log						&_logger;
		Epoll					_epoll;
		std::map<int, uint32_t>	_registred;
		
		void	_refresh_events()
		{
			events_t							events			= _epoll.accept();
			events_t::iterator					it				= events.begin();
			int									fd;
			std::map<int, uint32_t>::iterator	registeredItem;

			while (it != events.end())
			{
				fd = static_cast<event_data_t *>(it->data.ptr)->fd;
				registeredItem = _registred.find(fd);

				if (registeredItem != _registred.end())
					registeredItem->second |= it->events;
				else
					_logger.warn("Event happened on unregistred FD");
				++it;
			}
		}

	protected:
		AEpoll(Log &logger): _logger(logger), _epoll(logger)
		{}

		virtual ~AEpoll()
		{}

		bool	has(int fd)
		{
			return (_epoll.has(fd));
		}

		void	setup(int fd, event_type_t type, void *data = NULL, uint32_t events = EPOLLIN, uint32_t pre_events = 0)
		{
			if (_registred.insert(std::make_pair(fd, pre_events)).second)
				_epoll.add(fd, type, data, events);
		}

		void	cleanup(int fd)
		{
			_epoll.remove(fd);
			_registred.erase(fd);
		}

		void	clear_events(int fd)
		{
			std::map<int, uint32_t>::iterator	registeredItem;
			
			registeredItem = _registred.find(fd);

			if (registeredItem != _registred.end())
				registeredItem->second = 0;
			else
				_logger.warn("Use of unregistred FD");
		}

		void	clear_events(int fd, uint32_t events)
		{
			std::map<int, uint32_t>::iterator	registeredItem;
			
			registeredItem = _registred.find(fd);

			if (registeredItem != _registred.end())
				registeredItem->second &= ~events;
			else
				_logger.warn("Use of unregistred FD");
		}

		bool	await(int fd, uint32_t waited_events)
		{
			std::map<int, uint32_t>::iterator	registeredItem;
			
			_refresh_events();
			registeredItem = _registred.find(fd);

			if (registeredItem != _registred.end())
			{
				if (registeredItem->second & waited_events)
					return (true);
			}
			else
				_logger.warn("Await on unregistred FD");

			return (false);
		}
};

#endif