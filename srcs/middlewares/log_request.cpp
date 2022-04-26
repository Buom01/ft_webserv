# include "log_request.hpp"

bool	logRequest(Request &req, Response &res)
{
	if (req.finish())
		return (true);

	struct timespec		now = get_time();
	std::stringstream	infos;

	if (res.logger.options.verbose)
	{
		infos
			<< "(Req: "
			<< get_difftime_ms(req.start, req.generation_start)
			<< "ms; Page: "
			<< get_difftime_ms(req.generation_start, req.send_start)
			<< "ms; Res: "
			<< get_difftime_ms(req.send_start, now)
			<< "ms; Total: "
			<< get_difftime_ms(req.start, now)
			<< "ms)";
	}
	else
	{
		infos
			<< "(Total: "
			<< get_difftime_ms(req.start, now)
			<< "ms)";
	}

	res.logger.log(
		req.method_str,
		res.code,
		req.hostname.size() ? req.hostname : req.interface->ip,
		req.interface->port,
		req.trusted_complete_pathname,
		infos.str()
	);

	return (true);
}