/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 22:00:56 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <time.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write_headers.cpp"
# include "write_body.cpp"

bool	test_breakchain(Request &, Response &)
{
	static bool	one_time_on_two	= true;

	std::cout << "test_breakchain" << std::endl;

	return (one_time_on_two = !one_time_on_two);
}

bool	test_throwchain(Request &, Response &)
{
	throw new Serve::ServerSocketException("test_throwchain middleware");
}

bool	test_middleware(Request &, Response &res)
{
	res.code = C_OK;
	res.body << "TEST";

	return (true);
}

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);

	app.use(parseStartLine);
	app.use(parseRequestHeaders);

	// app.use(test_breakchain);
	app.use(test_middleware);
	// app.use(test_throwchain);

	app.use(addResponseHeaders, F_ALL);
	app.use(serializeHeaders, F_ALL);
	app.use(sendHeader, F_ALL);
	app.use(sendBodyMockupFunction, F_ALL);

	app.begin();
	while (1)
	{
		app.accept();
		sleep(1);
	}
}
