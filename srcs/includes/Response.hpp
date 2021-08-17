/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:50:37 by badam             #+#    #+#             */
/*   Updated: 2021/08/17 17:46:35 by badam            ###   ########.fr       */
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
		int						fd;
		Log						&logger;
		
		std::string				used_file;
		http_code_t				code;
		Header					headers;
		std::stringstream		body;
		bool					sent;

		Response(int connection, Log &_logger) :
			logger(_logger),
			fd(connection),
			used_file(""),
			code(C_NOT_IMPLEMENTED),
			headers(),
			body(""),
			sent(false)
		{}
};

#endif
