/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2021/08/01 03:45:02 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write.cpp"
# include "static.cpp"

int	main(void)
{
	Serve app;

	app.bind("0.0.0.0", 8888);

	app.use(&parseStartLine);
	app.use(&parseRequestHeaders);

	app.use(&serveStatic);
	
	app.use(&addResponseHeaders, F_ALL);
	app.use(&sendResponse, F_ALL);

	app.begin();
	while (1)
		app.accept();
}
