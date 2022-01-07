/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2022/01/07 18:12:53 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write.cpp"
# include "Static.cpp"

int	main(void)
{
	Serve 	app;
	Static	serveStatic;

	serveStatic.options.root				= "./staticfiles";
	serveStatic.options.directory_listing	= false;
	serveStatic.options.indexes.push_back("index.html");

	app.bind("0.0.0.0", 8888);

	app.use(parseStartLine);
	app.use(parseRequestHeaders);

	app.use(serveStatic);
	
	app.use(addResponseHeaders, F_ALL);
	app.use(sendResponse, F_ALL);

	app.begin();
	while (1)
		app.accept();
}
