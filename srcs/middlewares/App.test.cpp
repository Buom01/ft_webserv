/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2021/07/10 19:28:38 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "../serve/Serve.hpp"
#include "../serve/Request.hpp"
#include "../serve/Response.hpp"
#include "parseStartLine.hpp"

void	test_middleware(Request &req, Response &res)
{
	::send(res.fd, "SIMPLE\n", 7, MSG_DONTWAIT | MSG_EOR);
	res.logger.log(res.code, req.pathname);
	::close(res.fd);
}

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);

	app.use(&parseStartLine);
	app.use(&test_middleware);

	app.begin();
	while (app.accept())
		usleep(1);
}
