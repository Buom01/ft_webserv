#include "Static.hpp"

Static::Static()
{
	options.root = "./";
	options.directory_listing = true;
	options.indexes.push_back("index.html");
	options.indexes.push_back("index.htm");
	options.indexes.push_back("index.php");
}

Static::Static(options_t opts)
{
	options = opts;
}

Static::~Static() {}

bool	Static::serveDirectory(Response &res, const std::string &server_path, const std::string &client_path)
{
	if (!hasReadPermissions(server_path))
		return (false);
	std::vector<std::string>	files	= listFile(server_path);
	res.headers.set("Content-Type: text/html");
	res.code = C_OK;
	res.body = page(
		"List of file in " + client_path,
		title("List of files in " + client_path) +
		list(files, linkify)
	);
	return (true);
}

void	Static::serveFile(Response &res, const std::string &path)
{
	int	fd = open(path.c_str(), O_NONBLOCK | O_RDONLY);

	if (fd > 0)
	{
		res.response_fd = fd;
		res.code = C_OK;
		res.used_file = path;
	}
	else
		res.code = C_FORBIDDEN;
}

std::string Static::getIndex(const std::string &path, const options_t &options)
{
	std::vector<std::string>::const_iterator	it = options.indexes.begin();
	std::string									candidate;

	while (it != options.indexes.end())
	{
		candidate = path + *it;
		if (fileExists(candidate))
			return (candidate);
		++it;
	}
	return ("");
}

void	Static::redirect(Response &res, std::string path)
{
	res.headers.set("Location: " + path);
	res.code = C_MOVED_PERMANENTLY;
}

bool	Static::operator()(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}

	std::string	path = req.trusted_pathname;
	path.insert(0, options.root);
	if (isDirectory(path))
	{
		if (directoryExists(path))
		{
			std::string index = getIndex(path, options);

			if (index.size() > 0)
				serveFile(res, index);
			else if (options.directory_listing)
			{
				if (!serveDirectory(res, path, req.trusted_pathname))
					res.code = C_FORBIDDEN;
			}
			else
				res.code = C_FORBIDDEN;
		}
		else
			res.code = C_NOT_FOUND;
	}
	else if (directoryExists(path + "/") && getIndex(path + "/", options).size())
		redirect(res, req.pathname + "/");
	else if (fileExists(path))
		serveFile(res, path);
	else
		res.code = C_NOT_FOUND;
	return (true);
}
