/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eject.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 00:49:38 by badam             #+#    #+#             */
/*   Updated: 2022/04/03 21:40:40 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EJECT_CPP
# define __EJECT_CPP

# include <vector>
# include <string>
# include "IMiddleware.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Eject: public IMiddleware
{
	typedef	IMiddleware	_parent;

	public:
		size_t	max_payload_size;

		Eject(size_t _max_payload_size = 2048 * 200) :
			max_payload_size(_max_payload_size)
		{}
		
	public:
		bool	operator()(Request &req, Response &res)
		{
			std::vector<std::string>					header_values;
			std::vector<std::string>::const_iterator	header_values_it;

			if (res.code != C_NOT_IMPLEMENTED)
				return (true);

			if (req.http_version != "1.0" && req.http_version != "1.1")
			{
				res.code = C_HTTP_VERSION_NOT_SUPPORTED;
				return (true);
			}
			if (req.method == M_UNKNOWN)
			{
				res.code = C_METHOD_NOT_ALLOWED;
				return (true);
			}

			header_values = res.headers.headerValues("content-length");
			header_values_it = header_values.begin();
			while (header_values_it != header_values.end())
			{
				std::stringstream	sstream(*header_values_it);
				size_t				contentlength;

				sstream >> contentlength;

				if (contentlength > max_payload_size)
				{
					res.code = C_REQUEST_ENTITY_TOO_LARGE;
					return (true);
				}
				else
					++header_values_it;
			}

			return (true);
		}
};

#endif