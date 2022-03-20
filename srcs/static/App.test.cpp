/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:43:42 by badam             #+#    #+#             */
/*   Updated: 2022/03/18 07:32:18 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <time.h>
# include "Serve.hpp"
# include "read.cpp"
# include "write_headers.cpp"
# include "write_body.cpp"
# include "Static.cpp"


bool	hangForeverTest(Request &req, Response &res)
{
	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}
	return (false);
}

bool	throwErrorChain(Request &req, Response &res)
{
	throw Serve::ServerSocketException("throwErrorChain");
	
	return (true);
}

int	main(void)
{
	Serve 			app;
	Static			serveStatic;
	SendBodyFromFD	sendBodyFromFD(app.logger);

	serveStatic.options.root				= "./staticfiles";
	serveStatic.options.directory_listing	= false;
	serveStatic.options.indexes.push_back("index.html");

	app.bind("0.0.0.0", 8888);

	app.use(parseStartLine, F_ALL);
	app.use(parseRequestHeaders, F_ALL);

	// app.use(throwErrorChain);
	// app.use(hangForeverTest);
	// app.use(cgi);
	app.use(serveStatic);
	// app.use(error, F_ALL);
	
	app.use(addResponseHeaders, F_ALL);
	app.use(serializeHeaders, F_ALL);
	app.use(sendHeader, F_ALL);
	app.use(sendBodyFromBuffer, F_ALL);
	app.use(sendBodyFromFD, F_ALL);

	app.begin();
	while (app.alive())
	{
		app.accept();
		usleep(1);
	}
}
