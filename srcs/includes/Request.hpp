/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2022/04/07 17:09:06 by badam            ###   ########.fr       */
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
		std::string			client_ip;
		Log					&logger;
		uint32_t			events;
		bool				&alive;
		chain_state_t		*state;
		char				buff[SERVER_BUFFER_SIZE];
		
		std::string			method_str;
		method_t			method;
		std::string			raw_pathname;
		std::string			protocol;
		std::string			host;
		std::string			hostname;
		std::string			port;
		std::string			pathname;
		std::string			querystring;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;

		Request(int connection, std::string &_client_ip, uint32_t _events, bool &_alive, Log &_logger) :
			start(get_time()),
			fd(connection),
			client_ip(_client_ip),
			logger(_logger),
			events(_events),
			alive(_alive),
			state(NULL),
			method_str(""),
			method(M_UNKNOWN),
			raw_pathname(""),
			protocol(""),
			host(""),
			hostname(""),
			port(""),
			pathname(""),
			querystring(""),
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
