#include "body.hpp"

bool	_get_contentlength(Request &req)
{
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	header_values = req.headers.headerValues("content-length");
	header_values_it = header_values.begin();
	if (header_values.begin() == header_values.end())
		return (false);
	if (header_values_it != header_values.end())
	{
		std::stringstream	sstream(*header_values_it);
		sstream >> req.body_length;
	}
	return (true);
}

bool	_get_transferEncoding(Request &req, Response &res)
{
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	req.body_chuncked = false;
	header_values = req.headers.headerValues("transfer-encoding");
	header_values_it = header_values.begin();
	if (header_values.begin() == header_values.end())
		return (false);
	if (*header_values_it == "chunked")
	{
		req.body_length = 0;
		req.body_chuncked = true;
	}
	else
		res.code = C_BAD_REQUEST;
	return (true);
}

bool	_get_bondary(Request &req)
{
	std::string									boundary("boundary=");
	size_t										_pos(0), boundarySize(boundary.size());
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	header_values = req.headers.headerValues("content-type");
	header_values_it = header_values.begin();
	if (header_values.begin() == header_values.end())
		return (false);
	for (std::vector<std::string>::const_iterator it = header_values.begin(); it != header_values.end(); it++)
	{
		_pos = (*it).find(boundary);
		if (_pos != std::string::npos)
		{
			req.body_length = 0;
			req.body_boundary.assign((*it).substr(_pos).erase(0, boundarySize));
			if (req.body_boundary[0] == '"' && req.body_boundary[req.body_boundary.size() - 1] == '"')
				req.body_boundary = req.body_boundary.substr(1, req.body_boundary.size() - 2);
			req.body_boundary.append("--");
			break;
		}
	}
	return (true);
}

bool	body(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (req.finish())
		return (true);
	if (req.body_read_is_finished)
		return (true);
	if (!req.body_header_parsed)
	{
		_get_contentlength(req);
		_get_bondary(req);
		_get_transferEncoding(req, res);
		req.body_header_parsed = true;
		if (!req.body_chuncked && req.body_boundary.empty() && req.body_length <= 0)
			return (req.body_read_is_finished = true);
	}
	if (!req.await(EPOLLIN))
		return (false);

	std::string	line("");
	std::string	newline("");
	std::string	*newline_ptr	= &newline;

	if (req.body_chuncked == true)
	{
		while (get_next_line_string(req.fd, line, newline_ptr, req.buff))
		{
			if (!req.body_is_chunk)
			{
				req.body_chunk_size = 0;
				req.body_read_size = 0;
				std::stringstream sstream(line);
				sstream >> std::hex >> req.body_chunk_size;
				req.body_is_chunk = true;
			}
			else
			{
				if (req.body_chunk_size <= 0)
				{
					req.body_read_is_finished = true;
					break;
				}
				req.body.append(line);
				req.body_read_size += line.size();
				if (req.body_read_size >= req.body_chunk_size)
					req.body_is_chunk = false;
			}
		}
	}
	else
	{
		while (get_next_line_string(req.fd, line, newline_ptr, req.buff))
		{
			req.body.append(line);
			if (!req.body_boundary.empty())
			{
				if (
					(line.size() >= req.body_boundary.size()
					&& line.size() <= req.body_boundary.size() + 2)
					&& line.substr(line.size() - req.body_boundary.size(), req.body_boundary.size()) == req.body_boundary
				)
				{
					req.body_read_is_finished = true;
					break;
				}
				else
					req.body.append(newline);
			}
			else
			{
				req.body.append(newline);
				if (req.body.size() >= static_cast<size_t>(req.body_length))
				{
					req.body_read_is_finished = true;
					break;
				}
			}
		}
	}
	if (!req.body_read_is_finished)
		req.unfire(EPOLLIN);
	return (req.body_read_is_finished);
}
