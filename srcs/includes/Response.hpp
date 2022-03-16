/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:50:37 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 03:14:42 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <exception>
# include <sstream>
# include "Log.hpp"
# include "http.hpp"
# include "Header.hpp"
# include "BigBuffer.hpp"

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
		BigBuffer				body;
		char					*sending_chunk;
		size_t					sending_chunk_size;
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
			body(_logger, 2048),
			sending_chunk(NULL),
			sending_chunk_size(0),
			sent(false)
		{}

		virtual ~Response()
		{
			std::cout << "Response destoying" << std::endl;
		}
};

#endif
