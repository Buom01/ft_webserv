/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 22:47:57 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <time.h>
# include "Header.hpp"
# include "http.hpp"

typedef enum	chain_state_e
{
	CS_OTHER = 0,
	CS_AWAIT_EVENT
}				chain_state_t;

class Request
{
	public:
		clock_t				start;
		int					fd;
		uint32_t			events;
		chain_state_t		*state;
		char				buff[SERVER_BUFFER_SIZE];
		
		method_t			method;
		std::string			pathname;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;

		Request() :
			start(clock()),
			fd(0),
			events(0),
			state(NULL),
			method(M_UNKNOWN),
			pathname(""),
			trusted_pathname(""),
			http_version(""),
			headers()
		{
			bzero(&buff, sizeof(buff));
		}

		Request 	(const Request &rhs)
		{
			*this = rhs;
		}

		Request(int connection, uint32_t _events) :
			start(clock()),
			fd(connection),
			events(_events),
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

		Request 	&operator=(const Request &rhs)
		{
			if (this != &rhs)
			{
				start	= rhs.start;
				fd		= rhs.fd;
				events	= rhs.events;
				memcpy(buff, rhs.buff, SERVER_BUFFER_SIZE);

				method				= rhs.method;
				pathname			= rhs.pathname;
				trusted_pathname	= rhs.trusted_pathname;
				http_version		= rhs.http_version;
				headers				= rhs.headers;
			}

			return (*this);
		}

		bool	await(uint32_t _events)
		{
			if (events & _events)
			{
				*state = CS_OTHER;
				return (true);
			}
			else
			{
				std::cout << "Await..." << std::endl;
				events = events | _events;
				*state = CS_AWAIT_EVENT;
				return (false);
			}
		}

		bool	fire(uint32_t _events)
		{
			if (!(~events & _events))
			{
				std::cout << "Ignored fire" << std::endl;
				return (false);
			}
			else
			{
				std::cout << "Fire !" << std::endl;
				events = events | _events;
				return (true);
			}
		}

		void	unfire(uint32_t _events)
		{
			std::cout << "Unfire !" << std::endl;
			events = events &~ _events;
		}

		bool	closed()
		{
			return (events & EPOLLHUP);
		}
};

#endif
