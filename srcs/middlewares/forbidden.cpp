/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forbidden.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 07:54:52 by badam             #+#    #+#             */
/*   Updated: 2022/03/18 08:00:12 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FORBIDDEN_CPP
# define __FORBIDDEN_CPP

# include "Response.hpp"
# include "Request.hpp"

bool	forbidden(Request &, Response &res)
{
	res.code = C_FORBIDDEN;

	return (true);
}

#endif