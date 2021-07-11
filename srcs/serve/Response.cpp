/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:52:37 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:25:49 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../log/Log.hpp"
#include "Serve.hpp"


class Response
{
	public:
		const std::exception	*error;
		Log						&logger;
		server_address_s		addr;
		int						fd;
		
		http_code_t				code;
		Header					headers;
		std::string				body;
		bool					sent;

		Response(int connection, server_address_t &address, Log &_logger) :
			logger(_logger),
			addr(address),
			fd(connection),
			code(C_NOT_IMPLEMENTED),
			headers(),
			body(""),
			sent(false)
		{}
};
