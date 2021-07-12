/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 19:48:31 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serve.hpp"


void	test_middleware(Request &req, Response &res)
{
	::send(res.fd, "SIMPLE\n", 7, MSG_DONTWAIT | MSG_EOR);
	res.logger.log(res.code, req.pathname);
	::close(res.fd);
}

void	crash_middleware(Request &, Response &)
{
	std::string("abc").substr(10);
}

void	test_error_middleware(Request &req, Response &res)
{
	::send(res.fd, "ERROR\n", 6, MSG_DONTWAIT | MSG_EOR);
	res.logger.fail(req.pathname);
	res.logger.fail(res.error->what());
	::close(res.fd);
}

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);

	app.use(&test_middleware);
	app.use(&test_error_middleware, F_ERROR);
	//app.use(&crash_middleware, F_NORMAL);

	app.begin();
	while (1)
		app.accept();
}
