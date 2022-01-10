/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:50:37 by badam             #+#    #+#             */
/*   Updated: 2022/01/10 19:44:13 by badam            ###   ########.fr       */
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
		Log						*logger;
		const std::exception	*error;
		
		std::string				used_file;
		http_code_t				code;
		Header					headers;
		std::stringstream		body;
		bool					sent;
		
		Response() :
			fd(1),
			logger(NULL),
			error(NULL),
			used_file(""),
			code(C_NOT_IMPLEMENTED),
			headers(),
			body(""),
			sent(false)
		{}

		Response 	(const Response &rhs)
		{
			*this = rhs;
		}

		Response(int connection, Log &_logger) :
			fd(connection),
			logger(&_logger),
			error(NULL),
			used_file(""),
			code(C_NOT_IMPLEMENTED),
			headers(),
			body(""),
			sent(false)
		{}

		~Response()
		{}

		Response 	&operator=(const Response &rhs)
		{
			if (this != &rhs)
			{
				fd		= rhs.fd;
				logger	= rhs.logger;
				error	= rhs.error;

				used_file	= rhs.used_file;
				code		= rhs.code;
				headers		= rhs.headers;
				body << rhs.body.str();
				sent		= rhs.sent;
			}

			return (*this);
		}
};

#endif
