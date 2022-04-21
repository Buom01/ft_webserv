#include "forbidden.hpp"

bool	forbidden(Request &, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);

	res.code = C_FORBIDDEN;

	return (true);
}

bool	forbidden_method(Request &, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);

	res.code = C_METHOD_NOT_ALLOWED;

	return (true);
}
