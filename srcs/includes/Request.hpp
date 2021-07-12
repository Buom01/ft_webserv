/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 15:54:36 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "Header.hpp"
# include "Serve.hpp"

class Request
{
	public:
		server_address_s	addr;
		int					fd;
		char				buff[SERVER_BUFFER_SIZE];
		std::string			raw;
		
		method_t			method;
		std::string			pathname;
		std::string			http_version;
		Header				headers;
		std::string			content;

		Request(int connection, server_address_t &address) :
			addr(address),
			fd(connection),
			raw(),
			method(M_UNKNOWN),
			pathname(),
			http_version(),
			headers(),
			content()
		{
			bzero(&buff, sizeof(buff));
		}
};

#endif
