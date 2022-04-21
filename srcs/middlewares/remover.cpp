#include "remover.hpp"

Remover::Remover(std::string destination, std::string public_root = "")
{
	options.destination = destination;
	options.public_root = public_root;
}

Remover::~Remover() {}
	
bool	Remover::operator()(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (req.finish())
		return (true);
	std::string	remove_file_path	= concatPath(options.destination, "." + req.trusted_pathname);
	if (fileExists(remove_file_path))
		remove(remove_file_path.c_str());
	res.code = C_NO_CONTENT;
	return (true);
}
