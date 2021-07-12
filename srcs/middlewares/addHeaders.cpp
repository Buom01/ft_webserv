/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addHeaders.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 20:26:43 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 17:42:44 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADD_HEADERS_CPP
# define ADD_HEADERS_CPP
# include <string>
# include "Serve.hpp"

void	addHeaders(Request &, Response &res)
{
	Header				&h		= res.headers;
	std::stringstream	to_str;

	h.set("Server: FT_WebServ");
	
	to_str << res.body.length();
	h.set("Content-Length: " + to_str.str());

	if (res.code == C_OK && res.body.length() == 0)
		res.code = C_NO_CONTENT;
}

#endif
