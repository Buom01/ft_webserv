#include "write_body.hpp"

SendBodyFromFD::SendBodyFromFD(Log &logger): _parent(logger) {}

SendBodyFromFD::~SendBodyFromFD() {}
		
bool	SendBodyFromFD::operator()(Request &req, Response &res)
{
	ssize_t		read_ret = -1;
	ssize_t		send_ret = -1;
	static char	read_buffer[SERVER_BUFFER_SIZE];
	
	if (res.sent)
		return (true);
	if (!res.response_fd)
		return (true);
	else if (req.method == M_HEAD)
	{
		nothrow_close(res.response_fd);
		res.response_fd = 0;
		res.sent = true;
		return (true);
	}
	if (req.finish())
	{
		if (_parent::has(res.response_fd))
			_parent::cleanup(res.response_fd);
		return (true);
	}
		
	if (!req.await(EPOLLOUT))
		return (false);
	if (!_parent::has(res.response_fd))
		_parent::setup(res.response_fd, ET_BODY, NULL, EPOLLIN, EPOLLIN);  // Don't forget to revert that
		//_parent::setup(res.response_fd, ET_BODY, NULL, EPOLLIN);
	if (!_parent::await(res.response_fd, EPOLLIN))
	{
		if (_parent::await(res.response_fd, EPOLLRDHUP | EPOLLHUP))
			read_ret = 0;
		else
			return (false);
	}
	
	while (read_ret != 0 || send_ret != 0)
	{
		if (!res.response_fd_buff.empty())
		{
			send_ret = res.logger.logged_send(res.fd, res.response_fd_buff.c_str(), res.response_fd_buff.length(), MSG_NOSIGNAL);
			if (send_ret > 0)
				res.response_fd_buff.erase(0, send_ret);
			if (res.response_fd_buff.length())
			{
				req.unfire(EPOLLOUT);
				return (false);
			}
		}
		else
			send_ret = 0;

		if (read_ret != 0)
		{
			read_ret = read(res.response_fd, read_buffer, SERVER_BUFFER_SIZE);
			if (read_ret == -1)
			{
				_parent::clear_events(res.response_fd, EPOLLIN);
				return (false);
			}
			res.response_fd_buff.append(read_buffer, read_ret);
		}
	}

	// if (read_ret != 0 || send_ret != 0)
	// 	return (false);

	res.sent = true;
	_parent::cleanup(res.response_fd);
	nothrow_close(res.response_fd);
	res.response_fd = 0;
	return (true);
}

bool	sendBodyFromBuffer(Request &req, Response &res)
{
	ssize_t	send_ret = -1;
	size_t	write_size;

	if (res.sent)
		return (true);
	if (!res.body.length())
		return (true);
	else if (req.method == M_HEAD)
	{
		res.sent = true;
		return (true);
	}
	if (req.finish())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);
	while (res.body.length())
	{
		write_size	= min(SERVER_BUFFER_SIZE, res.body.length());
		send_ret = res.logger.logged_send(res.fd, res.body.c_str(), write_size, MSG_NOSIGNAL);
		
		if (send_ret > 0)
			res.body.erase(0, send_ret);
		if (send_ret < static_cast<ssize_t>(write_size))
		{
			req.unfire(EPOLLOUT);
			return (false);
		}
	}
	res.sent = true;
	return (true);
}
