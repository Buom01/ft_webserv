/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:50:37 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 06:05:40 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <exception>
# include <sstream>
# include "Log.hpp"
# include "http.hpp"
# include "Header.hpp"

class Response
{
	public:
		int						fd;
		Log						&logger;
		const std::exception	*error;
		
		std::string				used_file;
		http_code_t				code;
		Header					headers;
		std::string				headers_buff;
		bool					headers_sent;
		int						response_fd;
		std::string				response_fd_buff;
		std::string				body;
		size_t					send_chunksize;
		bool					sent;

		Response(int connection, Log &_logger) :
			fd(connection),
			logger(_logger),
			error(NULL),
			used_file(""),
			code(C_NOT_IMPLEMENTED),
			headers(),
			headers_buff(""),
			headers_sent(false),
			response_fd(0),
			response_fd_buff(""),
			body(""),
			send_chunksize(1024),
			sent(false)
		{}

		virtual ~Response()
		{}
};

#endif
