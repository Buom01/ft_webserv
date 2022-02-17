/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AEpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 22:54:02 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 23:16:32 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AEPOLL_HPP
# define AEPOLL_HPP

# include "IMiddleware.hpp"
# include "Epoll.hpp"

class AEpoll
{
	protected:
		Epoll	_epoll;

		std::set<int>	_registred;

		void	setup(int fd, event_type_t type, void *data, uint32_t events = EPOLLIN)
		{
			if (_registred.insert(fd).second)
				_epoll.add(fd, type, data, events)
		}

		void	cleanup(int fd)
		{
			_epoll.remove(fd);
		}

		void	await(int fd, uint32_t events)
		{
			events_t			events	= accept();
			events_t::iterator	it		= events.begin();

			while (it != events.end())
			{
				if (static_cast<event_data_t *>(it->data.ptr)->fd == fd)
				{
					if (it->events & events)
						return (true);
				}
				++it;
			}
			return (false);
		}
};

#endif