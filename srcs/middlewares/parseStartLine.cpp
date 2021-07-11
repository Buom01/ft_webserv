/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseStartLine.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 14:32:57 by badam             #+#    #+#             */
/*   Updated: 2021/07/11 14:42:36 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../serve/Serve.hpp"
#include "../serve/Request.hpp"
#include "../serve/Response.hpp"
#include "../parse/get_next_line/get_next_line.hpp"
#include "../parse/regex.hpp"

/*
** It parse the request's start line, getting the method, the pathname,
** and finally the HTTP version
*/
void	parseStartLine(Request &req, Response &)
{
	std::string	line;

	if (get_next_linestring(req.fd, line, req.buff, SERVER_BUFFER_SIZE) < 0)
		throw new Serve::ServerSocketException("Failed to read start line");
	std::cout << line << std::endl;
}
