/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addHeaders.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 20:26:43 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 01:46:17 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serve/Serve.hpp"

void	addHeaders(Request &req, Response &res)
{
	res.headers.Set("Server", "FT_WebServ_by_cbertran_and_badam");
	res.headers.Set("Content-Length", res.body.length());
	if (res.code == C_OK && res.body.length() == 0)
		res.code = C_NO_CONTENT;
}
