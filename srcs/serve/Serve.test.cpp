/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2022/03/10 22:52:10 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"
# include "Response.hpp"
#include "Serve.hpp"


bool	test_middleware(Request &req, Response &res)
{
	res.code = C_OK;
	res.body << "SIMPLE\n";
	::send(res.fd, res.body.str().c_str(), res.body.length(), MSG_DONTWAIT | MSG_EOR);
	::close(res.fd);
	res.sent = true;
	res.logger.log(res.code, req.pathname);

	return (true);
}

bool	crash_middleware(Request &, Response &)
{
	std::string("abc").substr(10);

	return (true);
}

bool	test_error_middleware(Request &req, Response &res)
{
	::send(res.fd, "ERROR\n", 6, MSG_DONTWAIT | MSG_EOR);
	res.logger.fail(req.pathname);
	res.logger.fail(res.error->what());
	::close(res.fd);

	return (true);
}

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);
	app.bind("localhost", 1337);
	app.bind("failtestip", 8080);

	app.use(test_middleware);
	app.use(test_error_middleware, F_ERROR);
	//app.use(crash_middleware, F_NORMAL);

	app.begin();
	while (1)
	{
		app.accept();
		app.retake();
	}
}
