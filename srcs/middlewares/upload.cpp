#include "upload.hpp"

Upload::Upload(Log &logger, std::string destination, std::string public_root = ""):
	_parent(logger),
	_logger(logger)
{
	options.destination = destination;
	options.public_root = public_root;
}

Upload::~Upload() {}
	
bool	Upload::_eject_contentrange(Request &req, Response &res)
{
	std::vector<std::string>					header_values;
	std::vector<std::string>::const_iterator	header_values_it;

	header_values = req.headers.headerValues("content-range");
	header_values_it = header_values.begin();
	if (header_values.begin() != header_values.end())
	{
		res.code = C_BAD_REQUEST;
		return (false);
	}
	return (true);
}
		
bool	Upload::operator()(Request &req, Response &res)
{
	ssize_t	write_chunksize, write_ret = -1;
	
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (req.finish())
	{
		if (req.upload_fd)
		{
			if (_parent::has(req.upload_fd))
				_parent::cleanup(req.upload_fd);
			nothrow_close(req.upload_fd);
			req.upload_fd = 0;
			if (fileExists(req.upload_filename_tmp))
				remove(req.upload_filename_tmp.c_str());
		}
		return (true);
	}
	if (!req.upload_fd)
	{
		if (!_eject_contentrange(req, res))
			return (true);
		req.upload_filename = concatPath(options.destination, "." + req.trusted_pathname);
		req.upload_filename_tmp = req.upload_filename + ".tmp";
		if (fileExists(req.upload_filename_tmp))
			remove(req.upload_filename_tmp.c_str());
		req.upload_fd = open(req.upload_filename_tmp.c_str(), O_NONBLOCK | O_WRONLY | O_CREAT);
		if (req.upload_fd < 0)
		{
			_logger.warn("Failed to open temp upload file " + req.upload_filename_tmp);
			res.code = C_INTERNAL_SERVER_ERROR;
			return (true);
		}
	}
	if (!_parent::has(req.upload_fd))
		_parent::setup(req.upload_fd, ET_BODY, NULL, EPOLLOUT);
	if (!_parent::await(req.upload_fd, EPOLLOUT))
		return (false);
	while (write_ret != 0)
	{
		write_chunksize = req.upload_chunksize > req.body.length() ? req.body.length() : req.upload_chunksize;
		write_ret = write(req.upload_fd, req.body.c_str(), write_chunksize);
		if (write_ret > 0)
			req.body.erase(0, write_ret);
		if (write_ret < write_chunksize)
		{
			_parent::clear_events(req.upload_fd, EPOLLOUT);
			return (false);
		}
	}
	_parent::cleanup(req.upload_fd);
	nothrow_close(req.upload_fd);
	req.upload_fd = 0;
	// if (fileExists(req.upload_filename))
	// {
	// 	remove(req.upload_filename.c_str());
	// 	res.code = C_NO_CONTENT;
	// }
	// else
	// 	res.code = C_CREATED;
	if (fileExists(req.upload_filename))
		remove(req.upload_filename.c_str());
	res.code = C_CREATED;
	rename(req.upload_filename_tmp.c_str(), req.upload_filename.c_str());
	if (options.public_root.length())
	{
		res.headers.add("Content-Location: " + sanitizeRelativePath(concatPath(options.public_root, "." + req.trusted_pathname)));
	}
	return (true);
}
