/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2021/08/17 17:49:26 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "Header.hpp"
# include "Serve.hpp"

class Request
{
	public:
		server_bind_t		bind;
		int					fd;
		char				buff[SERVER_BUFFER_SIZE];
		
		method_t			method;
		std::string			pathname;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;

		Request(int connection, server_bind_t &binding) :
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
};

#endif
