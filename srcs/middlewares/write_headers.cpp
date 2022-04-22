#include "write_headers.hpp"

bool	addResponseHeaders(Request &req, Response &res)
{
	Header		&h		= res.headers;
	off_t		length	= 0;

	h.set("Server: FT_WebServ");

	if (res.body.length() > 0)
		length = static_cast<intmax_t>(res.body.length());
	else if (res.response_fd && res.response_fd)
		length = fdFileSize(res.response_fd) - res.response_fd_header_size;  // Works on stdin too

	if (length >= 0)
	{
		std::stringstream	to_str;
		
		to_str << length;
		h.set("Content-Length: " + to_str.str());
	}
	else
		req.keep_alive = false;

	if (req.body_read_size)
	{
		std::stringstream	to_str;

		to_str << req.body_read_size;
		h.set("Content-Length: " + to_str.str());
	}

	if (req.headers.header("Connection", true) == "close")
		req.keep_alive = false;

	if (req.keep_alive && req.alive)
	{
		h.set("Connection: keep-alive");
		h.set("Keep-Alive: timeout=250");
	}
	else
	{
		h.set("Connection: close");
	}
	
	return (true);
}

bool	serializeHeaders(Request &req, Response &res)
{
	if (req.finish())
		return (true);

	std::stringstream			startline;
	std::vector<std::string>	ListHeaders = res.headers.headers();
	
	res.headers_buff.reserve(120);
	startline << "HTTP/1.1 " << res.code << "\r\n";
	res.headers_buff.append(startline.str());

	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
	{
		res.headers_buff.append(*it);
		res.headers_buff.append("\r\n");
	}
	res.headers_buff.append("\r\n");

	std::stringstream	infos;
	infos << get_elasped_ms(req.start);
	infos << "ms";
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

bool	sendHeader(Request &req, Response &res)
{
	ssize_t	send_ret = -1;
	size_t	write_size;

	if (req.finish())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	while (res.headers_buff.length())
	{
		write_size	= min(SERVER_BUFFER_SIZE, res.headers_buff.length());
		send_ret = res.logger.logged_send(res.fd, res.headers_buff.c_str(), write_size, MSG_NOSIGNAL);

		if (send_ret > 0)
			res.headers_buff.erase(0, send_ret);
		if (send_ret < static_cast<ssize_t>(write_size))
		{
			req.unfire(EPOLLOUT);
			return (false);
		}
	}

	res.headers_sent = true;
	return (true);
}
