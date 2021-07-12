/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addHeaders.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 20:26:43 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 13:43:29 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serve/Serve.hpp"

void	addHeaders(Request &req, Response &res)
{
	Header				&h		= res.headers;
	std::stringstream	to_str;

	h.set("Server: FT_WebServ");
	
	to_str << res.body.length();
	h.set(h.formate("Content-Length", to_str.str()));

	if (res.code == C_OK && res.body.length() == 0)
		res.code = C_NO_CONTENT;
}
