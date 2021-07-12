/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 16:08:34 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 17:34:39 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include <errno.h>

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
	logger.warn("Simple warn", EBADMSG);
	logger.fail("Critical error");
	logger.fail("Critical error", EBADMSG);
}
