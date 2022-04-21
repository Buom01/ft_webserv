#include "webserv.hpp"

Serve 			app;

void	stop_signal(int)
{
	app.stop();
}

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

bool	throwErrorChain(Request &, Response &)
{
	throw Serve::ServerSocketException("throwErrorChain");
	
	return (true);
}

int	main(void)
{
	Eject			eject;
	Static			serveStatic;
	SendBodyFromFD	sendBodyFromFD(app.logger);
	Error			error(app.logger);
	Mimetypes		mimetypes;
	Upload			upload(app.logger, "/tmp", "/test");

	serveStatic.options.root				= "./staticfiles";
	serveStatic.options.directory_listing	= true;
	serveStatic.options.indexes.push_back("index.html");

	for (int i = 300; i < 600; ++i)
		error.add(i, "errorpages/generic_demo.html");

	mimetypes.add("html", "text/html");

	app.bind("0.0.0.0", 8888);

	app.use(parseStartLine, F_ALL);
	app.use(parseRequestHeaders, F_ALL);
	app.use(eject);

	app.use(upload, F_NORMAL, M_PUT);
	app.use(serveStatic);
	app.use(error, F_ALL);
	
	app.use(mimetypes, F_ALL);
	app.use(addResponseHeaders, F_ALL);
	app.use(serializeHeaders, F_ALL);
	app.use(sendHeader, F_ALL);
	app.use(sendBodyFromBuffer, F_ALL);
	app.use(sendBodyFromFD, F_ALL);

   signal(SIGINT, stop_signal);

	app.begin();
	while (app.alive())
	{
		app.accept();
		usleep(10);
	}
}
