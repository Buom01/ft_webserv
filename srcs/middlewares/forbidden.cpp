/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forbidden.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 07:54:52 by badam             #+#    #+#             */
/*   Updated: 2022/04/12 03:02:43 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FORBIDDEN_CPP
# define __FORBIDDEN_CPP

# include "Response.hpp"
# include "Request.hpp"

bool	forbidden(Request &, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);

	res.code = C_FORBIDDEN;

	return (true);
}

bool	forbidden_method(Request &, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);

	res.code = C_METHOD_NOT_ALLOWED;

	return (true);
}

#endif
