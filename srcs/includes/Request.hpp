/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2022/03/18 07:08:34 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <time.h>
# include "Log.hpp"
# include "Header.hpp"
# include "http.hpp"
# include "utils.hpp"

typedef enum	chain_state_e
{
	CS_OTHER = 0,
	CS_AWAIT_EVENT
}				chain_state_t;

class Request
{
	public:
		struct timespec		start;
		int					fd;
		Log					&logger;
		uint32_t			events;
		bool				&alive;
		chain_state_t		*state;
		char				buff[SERVER_BUFFER_SIZE];
		
		method_t			method;
		std::string			pathname;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;

		Request(int connection, uint32_t _events, bool &_alive, Log &_logger) :
			start(get_time()),
			fd(connection),
			logger(_logger),
			events(_events),
			alive(_alive),
			state(NULL),
			method(M_UNKNOWN),
			pathname(""),
			trusted_pathname(""),
			http_version(""),
			headers()
		{
			bzero(&buff, sizeof(buff));
		}

		virtual ~Request()
		{}

		bool	await(uint32_t _events)
		{
			if (events & _events)
			{
				*state = CS_OTHER;
				return (true);
			}
			else
			{
				*state = CS_AWAIT_EVENT;
				return (false);
			}
		}

		bool	fire(uint32_t _events)
		{
			if (!(~events & _events))
			{
				if (*state == CS_AWAIT_EVENT)
				{
					logger.warn("Were waiting on an unused event");
					*state = CS_OTHER;
				}
				return (false);
			}
			else
			{
				events = events | _events;
				return (true);
			}
		}

		void	unfire(uint32_t _events)
		{
			events = events &~ _events;
		}

		bool	timeout()
		{
			return (get_elasped_ns(start) >= (int64_t)30 * 1000000000);
		}

		bool	closed()
		{
			return (events & EPOLLHUP);
		}

		bool	finish()
		{
			return (closed() || (!alive && timeout()));
		}
};

#endif
