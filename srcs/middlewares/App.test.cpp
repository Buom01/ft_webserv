/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2021/07/27 19:07:57 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write.cpp"

void	test_middleware(Request &, Response &res)
{
	res.code = C_OK;
	res.body = "TEST";
}

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);

	app.use(&parseStartLine);
	app.use(&parseRequestHeaders);

	app.use(&test_middleware);
	
	app.use(&addResponseHeaders, F_ALL);
	app.use(&sendResponse, F_ALL);

	app.begin();
	while (1)
		app.accept();
}
