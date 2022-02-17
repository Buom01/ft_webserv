/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AEpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:54:02 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 23:54:54 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AEPOLL_HPP
# define AEPOLL_HPP

# include <set>
# include "IMiddleware.hpp"
# include "Epoll.hpp"

class AEpoll: public IMiddleware
{
	protected:
		Epoll	_epoll;

		AEpoll(Log &logger): _epoll(logger)
		{}

		virtual ~AEpoll()
		{}


		std::set<int>	_registred;

		void	setup(int fd, event_type_t type, void *data, uint32_t events = EPOLLIN)
		{
			if (_registred.insert(fd).second)
				_epoll.add(fd, type, data, events);
		}

		void	cleanup(int fd)
		{
			_epoll.remove(fd);
		}

		bool	await(int fd, uint32_t waited_events)
		{
			events_t			events	= _epoll.accept();
			events_t::iterator	it		= events.begin();

			while (it != events.end())
			{
				if (static_cast<event_data_t *>(it->data.ptr)->fd == fd)
				{
					if (it->events & waited_events)
						return (true);
				}
				++it;
			}
			return (false);
		}
};

#endif