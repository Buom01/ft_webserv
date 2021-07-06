/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 16:08:34 by badam             #+#    #+#             */
/*   Updated: 2021/07/06 23:27:07 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"


int	main(void)
{
	Log	logger = Log();

	logger.greeting("localhost", 80);
	logger.log(200, "/found");
	logger.log(200, "/found/yay", "Yay!");
	logger.log(404, "/not-found");
	logger.log(404, "/not-found/lost", "File has been lost");
	logger.log(500, "/server-error");
	logger.log(500, "/server-error/oops", "Oops!");
	logger.warn("Simple warn");
	logger.fail("Critical error");
}
