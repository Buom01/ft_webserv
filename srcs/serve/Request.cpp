/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:28:53 by badam             #+#    #+#             */
/*   Updated: 2021/07/09 21:30:04 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serve.hpp"
#include "Serve_common.hpp"

class Request
{
	public:
		server_address_s	addr;
		int					fd;
		char				buff[SERVER_BUFFER_SIZE];
		std::string			raw;
		
		method_t			method;
		std::string			pathname;
		headers_t			headers;
		std::string			content;

		Request(int connection, server_address_t &address) :
			addr(address),
			fd(connection),
			method(M_UNKNOWN)
		{
			bzero(&buff, sizeof(buff));
		}
};
