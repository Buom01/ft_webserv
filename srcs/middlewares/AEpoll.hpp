/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AEpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:54:02 by badam             #+#    #+#             */
/*   Updated: 2022/03/10 23:25:09 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AEPOLL_HPP
# define AEPOLL_HPP

# include <set>
# include "IMiddleware.hpp"
# include "Epoll.hpp"

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

		void	setup(int fd, event_type_t type, void *data, uint32_t events = EPOLLIN)
		{
			if (_registred.insert(std::make_pair(fd, 0)).second)
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