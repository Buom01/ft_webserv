/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2022/01/10 23:04:38 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <time.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write.cpp"

bool	test_breakchain(Request &req, Response &)
{
	static bool	one_time_on_two	= true;

	std::cout << "test_breakchain" << std::endl;

	return (one_time_on_two = !one_time_on_two);
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

	app.use(test_breakchain);
	app.use(test_middleware);
	
	app.use(addResponseHeaders, F_ALL);
	app.use(sendResponse, F_ALL);

	app.begin();
	while (1)
	{
		app.accept();
		app.retake();
	}
}
