#include "gnl.hpp"

size_t	get_line_end(std::string &buff)
{
	return (buff.find("\r\n"));
}

bool	get_line(std::string &line, std::string *newline, std::string &buff, bool force_end)
{
	size_t	end	= get_line_end(buff);

	if (end != std::string::npos)
	{
		line.append(buff, 0, end);
		buff.erase(0, end + 2);
		if (newline)
			*newline = "\r\n";
		return (true);
	}
	else if (force_end)
	{
		line.append(buff);
		buff.clear();
		if (newline)
			*newline = "";
		return (true);
	}
	else
		return (false);
}

bool	get_next_line_string(int fd, std::string &line, std::string *newline, std::string &buff)
{
	char	read_buff[SERVER_BUFFER_SIZE];
	ssize_t	read_ret						= -1;
	
	line.clear();
	while (!get_line(line, newline, buff, read_ret == 0))
	{
		read_ret = read(fd, read_buff, SERVER_BUFFER_SIZE);
		if (read_ret <= 0)
			return (false);
		else if (read_ret > 0)
			buff.append(read_buff, read_ret);
	}
	return (true);
}