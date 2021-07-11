/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:52:37 by badam             #+#    #+#             */
/*   Updated: 2021/07/11 18:32:53 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../log/Log.hpp"
#include "Serve.hpp"
#include "Serve_common.hpp"

class Response
{
	public:
		const std::exception	*error;
		Log						&logger;
		server_address_s		addr;
		int						fd;
		
		http_code_t				code;
		headers_t				headers;
		std::string				body;
		bool					sent;

		Response(int connection, server_address_t &address, Log &_logger) :
			logger(_logger),
			addr(address),
			fd(connection),
			code(C_UNKNOWN),
			headers(),
			body(""),
			sent(false)
		{}
};
