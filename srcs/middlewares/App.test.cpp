/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2021/07/12 17:24:14 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include "../serve/Serve.hpp"
# include "parseStartLine.hpp"
# include "addHeaders.hpp"
# include "sendResponse.hpp"

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
	app.use(&test_middleware);
	app.use(&addHeaders, F_ALL);
	app.use(&sendResponse, F_ALL);

	app.begin();
	while (app.accept())
		usleep(1);
}
