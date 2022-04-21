#include "eject.hpp"

bool	eject(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);

	if (req.http_version != "1.0" && req.http_version != "1.1")
	{
		res.code = C_HTTP_VERSION_NOT_SUPPORTED;
		return (true);
	}
	if (req.method == M_UNKNOWN)
	{
		res.code = C_METHOD_NOT_ALLOWED;
		return (true);
	}

	return (true);
}

EjectBody::EjectBody(size_t _max_body_size) : max_body_size(_max_body_size) {}

EjectBody::~EjectBody() {}

bool EjectBody::operator()(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (!req.body_size_valid && req.body.size() > max_body_size)
		res.code = C_REQUEST_ENTITY_TOO_LARGE;
	req.body_size_valid	= true;
	return (true);
}
