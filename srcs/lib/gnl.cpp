#include "gnl.hpp"

size_t	get_line_end(std::string &buff)
{
	return (buff.find("\r\n"));
}

bool	get_line(std::string &line, std::string &buff)
{
	size_t	end	= get_line_end(buff);

	if (end != std::string::npos)
	{
		line.append(buff, 0, end);
		buff.erase(0, end + 2);
		return (true);
	}
	else
		return (false);
}

bool	get_next_line_string(int fd, std::string &line, std::string &buff, Log &logger)
{
	static char	read_buff[SERVER_BUFFER_SIZE];
	ssize_t	read_ret						= -1;
	
	line.clear();
	while (!get_line(line, buff))
	{
		read_ret = read(fd, read_buff, SERVER_BUFFER_SIZE);
		if (read_ret <= 0)
		{
			// if (read_ret < 0 && errno != EAGAIN)
			// 	logger.warn("Read fail for unexpected reason", errno);
			return (false);
		}
		else if (read_ret > 0)
		{
			buff.append(read_buff, read_ret);
			// logger.log_flux(fd, read_buff, read_ret, "READ");
		}
	}
	logger.logged_GNL(fd, line + "\r\n");
	return (true);
}