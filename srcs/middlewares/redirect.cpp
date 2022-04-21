#include "redirect.hpp"

Redirect::Redirect(http_code_t code, std::string &location)
{
	options.code = code;
	options.location = location;
}

Redirect::Redirect(options_t opts)
{
	options = opts;
}

Redirect::~Redirect() {}

bool	Redirect::operator()(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	std::string	newLocation(options.location);
	replace_all(&newLocation, "$request_uri", req.pathname);
	res.headers.set("Location: " + newLocation);
	res.code = options.code;
	return (true);
}
