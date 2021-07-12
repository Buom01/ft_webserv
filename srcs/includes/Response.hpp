/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:50:37 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 17:27:17 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <exception>
# include "Log.hpp"
# include "Serve.hpp"
# include "Header.hpp"

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

#endif
