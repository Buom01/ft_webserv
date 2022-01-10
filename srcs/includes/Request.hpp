/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2022/01/10 22:51:45 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <time.h>
# include "Header.hpp"
# include "http.hpp"

class Request
{
	public:
		clock_t				start;
		server_bind_t		bind;
		int					fd;
		char				buff[SERVER_BUFFER_SIZE];
		
		method_t			method;
		std::string			pathname;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;

		Request() :
			start(clock()),
			fd(0),
			method(M_UNKNOWN),
			pathname(""),
			trusted_pathname(""),
			http_version(""),
			headers()
		{
			bzero(&buff, sizeof(buff));
			bzero(&bind, sizeof(bind));
		}

		Request 	(const Request &rhs)
		{
			*this = rhs;
		}

		Request(int connection, server_bind_t &binding) :
			start(clock()),
			bind(binding),
			fd(connection),
			method(M_UNKNOWN),
			pathname(""),
			trusted_pathname(""),
			http_version(""),
			headers()
		{
			bzero(&buff, sizeof(buff));
		}

		~Request()
		{}

		Request 	&operator=(const Request &rhs)
		{
			if (this != &rhs)
			{
				start	= rhs.start;
				bind	= rhs.bind;
				fd		= rhs.fd;
				memcpy(buff, rhs.buff, SERVER_BUFFER_SIZE);

				method				= rhs.method;
				pathname			= rhs.pathname;
				trusted_pathname	= rhs.trusted_pathname;
				http_version		= rhs.http_version;
				headers				= rhs.headers;
			}

			return (*this);
		}

};

#endif
