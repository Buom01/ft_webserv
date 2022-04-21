#include "read.hpp"

bool	parseStartLine(Request &req, Response &res)
{
	std::string	line;
	Regex		regex;

	if (req.finish() || req.closed())
		return (true);
	if (req.timeout())
	{
		std::stringstream	warn;
		
		warn << "Timeout while reading StartLine on FD: " << req.fd;
		res.logger.warn(warn.str());
		req.keep_alive = false;
		return (true);
	}
	if (!req.await(EPOLLIN))
		return (false);

	if (!get_next_line_string(req.fd, line, req.buff, res.logger))
	{
		req.unfire(EPOLLIN);
		return (false);
	}
	if (line.empty())
	{
		res.logger.warn("Empty line before StartLine: Did you miss to read end of the previous request ?");
		return (false);
	}

	regex.exec(line, "^([A-Z]{3,10})\\ ([^\\ ]+)\\ HTTP\\/([0-9\\.]{3,6})$", GLOBAL_FLAG);
	if (regex.size() != 3)
	{
		res.code = C_BAD_REQUEST;
		return (true);
	}

	std::string &method = regex.match()[0].occurence;
	std::string &pathname = regex.match()[1].occurence;
	std::string &http_version = regex.match()[2].occurence;

	if (method == "GET")
		req.method = M_GET;
	else if (method == "HEAD")
		req.method = M_HEAD;
	else if (method == "POST")
		req.method = M_POST;
	else if (method == "PUT")
		req.method = M_PUT;
	else if (method == "DELETE")
		req.method = M_DELETE;
	else if (method == "CONNECT")
		req.method = M_CONNECT;
	else if (method == "OPTIONS")
		req.method = M_OPTIONS;
	else if (method == "TRACE") 
		req.method = M_TRACE;

	req.method_str = method;
	req.raw_pathname = pathname;
	req.http_version = http_version;

	if (pathname[0] == '/' || pathname[0] == '*')
	{
		Regex   regex;

		regex.exec(pathname, "^(\\/[^?]+)(\\?.*)?$");
		if (regex.size() >= 2)
		{
			req.pathname = regex.match()[0].occurence;
			req.querystring = regex.match()[1].occurence;
		}
		else
			req.pathname = pathname;
		req.trusted_complete_pathname = sanitizeRelativePath(req.pathname);
	}
	else
	{
		URL	uri(pathname);

		req.protocol = uri.protocol();
		req.host = uri.host();
		req.hostname = uri.hostname();
		req.port = uri.port();
		req.pathname = uri.pathname();
		req.username = uri.username();
		req.password = uri.password();
		req.querystring = uri.search();
		req.trusted_complete_pathname = sanitizeRelativePath(req.pathname);
	}

	return (true);
}

bool	fulfillHostFromHeader(Request &req, Response &)
{
	Regex   									regex;
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	header_values = req.headers.headerValues("host");
	header_values_it = header_values.begin();

	if (header_values_it != header_values.end())
	{
		std::string	host(*header_values_it);

		regex.exec(host, "^([^:]+)(:([0-9]{1,16}))?$", GLOBAL_FLAG);
		req.host = host;

		if (regex.size() >= 1)
			req.hostname = regex.match()[0].occurence;
		if (regex.size() >= 3)
			req.port = regex.match()[2].occurence;
	}

	return (true);
}

bool	parseRequestHeaders(Request &req, Response &res)
{
	std::string	line;

	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}
	if (!req.await(EPOLLIN))
		return (false);

	while (get_next_line_string(req.fd, line, req.buff, res.logger))
	{
		if (!line.length())
		{
			fulfillHostFromHeader(req, res);
			return (true);
		}
		else
			req.headers.set(line);
	}

	req.unfire(EPOLLIN);

	return (false);
}
