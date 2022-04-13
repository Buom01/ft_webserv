/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/04/12 22:57:34 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UPLOAD_CPP
# define __UPLOAD_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "AEpoll.hpp"
# include "utils.hpp"
# include "File.hpp"

class Body: public AEpoll
{
	typedef	AEpoll	_parent;

	protected:
		Log		&_logger;

		typedef struct s_body {
			FILE	*file;
			int		fd;
		};

		s_body	body;

	public:
		Body(Log &logger): _parent(logger), _logger(logger)
		{
			body.file = tmpfile();
			body.fd = fileno(body.file);
		}

		virtual ~Body()
		{}
	
	protected:
		bool	_get_contentlength(Request &req, Response &res)
		{
			std::vector<std::string>					header_values;
			std::vector<std::string>::const_iterator	header_values_it;

			header_values = req.headers.headerValues("content-length");
			header_values_it = header_values.begin();
			if (header_values.begin() == header_values.end())
			{
				res.code = C_LENGTH_REQUIRED;
				return (false);
			}
			if (header_values_it != header_values.end())
			{
				std::stringstream	sstream(*header_values_it);

				sstream >> req.body_remainingsize;
			}

			return (true);
		}

		bool	_eject_contentrange(Request &req, Response &res)
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
		
	public:
		bool	operator()(Request &req, Response &res)
		{
			size_t	read_chunksize;
			char	read_buffer[req.body_chunksize];
			ssize_t	read_ret							= -1;
			ssize_t	write_ret							= -1;
			
			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);

			if (req.finish())
			{
				if (req.body)
				{
					if (_parent::has(req.body))
						_parent::cleanup(req.body);
					nothrow_close(req.body);
					req.body = 0;
				}
				return (true);
			}

			if (!req.body)
			{
				if (!_get_contentlength(req, res))
					return (true);
				if (!_eject_contentrange(req, res))
					return (true);
				
				if (req.body < 0)
				{
					_logger.warn("Failed to open temp body file");
					res.code = C_INTERNAL_SERVER_ERROR;
					return (true);
				}
			}
				
			if (!req.await(EPOLLIN))
				return (false);
			if (!_parent::has(req.body))
				_parent::setup(req.body, ET_BODY, NULL, EPOLLOUT);
			if (!_parent::await(req.body, EPOLLOUT))
				return (false);

			if (write_ret != 0 && strlen(req.buff))
			{
				ssize_t	new_len;

				write_ret	= write(req.body, req.buff, strlen(req.buff));
				new_len		= static_cast<ssize_t>(strlen(req.buff)) - write_ret;
				if (write_ret > 0)
				{
					memmove(req.buff, req.buff + write_ret, new_len);
					req.buff[new_len + 1] = '\0';
				}
				if (new_len)
				{
					_parent::clear_events(req.body, EPOLLOUT);
					return (false);	
				}
			}
			
			while (read_ret != 0 || write_ret != 0)
			{
				write_ret = write(req.body, req.body_buff.c_str(), req.body_buff.length());
				if (write_ret > 0)
					req.body_buff.erase(0, write_ret);
				if (req.body_buff.length())
				{
					_parent::clear_events(req.body, EPOLLOUT);
					return (false);
				}

				read_chunksize = req.body_remainingsize < req.body_chunksize ? req.body_remainingsize : req.body_chunksize;
				read_ret = read(req.fd, read_buffer, read_chunksize);
				if (read_ret == -1)
				{
					req.unfire(EPOLLIN);
					return (false);
				}
				req.body_remainingsize -= read_ret;
				req.body_buff.append(read_buffer, read_ret);
			}

			_parent::cleanup(req.body);
			/*nothrow_close(req.body);
			req.body = 0;
			
			if (req.body.st_nlink == 0)
				res.code = C_NO_CONTENT;
			else
				res.code = C_CREATED;
			*/
			return (true);
		}

};

#endif
