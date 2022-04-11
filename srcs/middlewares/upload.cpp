/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/04/12 01:16:51 by badam            ###   ########.fr       */
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

class Upload: public AEpoll
{
	typedef	AEpoll	_parent;

	protected:
		Log		&_logger;

	public:
		typedef struct	options_s
		{
			std::string	destination;
			std::string	public_root;
		}				options_t;
		
		options_t		options;

		Upload(Log &logger, std::string destination, std::string public_root = ""):
			_parent(logger),
			_logger(logger)
		{
			options.destination = destination;
			options.public_root = public_root;
		}

		virtual ~Upload()
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

				sstream >> req.upload_remainingsize;
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
			char	read_buffer[req.upload_chunksize];
			ssize_t	read_ret							= -1;
			ssize_t	write_ret							= -1;
			
			if (res.sent)
				return (true);

			if (req.finish() || req.closed())
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
				if (!_get_contentlength(req, res))
					return (true);
				if (!_eject_contentrange(req, res))
					return (true);
				
				req.upload_filename = concatPath(options.destination, "." + req.trusted_pathname);
				req.upload_filename_tmp = req.upload_filename + ".tmp";
				if (fileExists(req.upload_filename_tmp))
					remove(req.upload_filename_tmp.c_str());
				req.upload_fd = open(req.upload_filename_tmp.c_str(), O_NONBLOCK | O_WRONLY | O_CREAT);
				if (req.upload_fd < 0)
					_logger.warn("Failed to open temp upload file");
			}
				
			if (!req.await(EPOLLIN))
				return (false);
			if (!_parent::has(req.upload_fd))
				_parent::setup(req.upload_fd, ET_BODY, NULL, EPOLLOUT);
			if (!_parent::await(req.upload_fd, EPOLLOUT))
				return (false);
			
			while (read_ret != 0 || write_ret != 0)
			{
				write_ret = write(req.upload_fd, req.upload_fd_buff.c_str(), req.upload_fd_buff.length());
				if (write_ret > 0)
					req.upload_fd_buff.erase(0, write_ret);
				if (req.upload_fd_buff.length())
				{
					_parent::clear_events(req.upload_fd, EPOLLOUT);
					return (false);
				}

				read_chunksize = req.upload_remainingsize < req.upload_chunksize ? req.upload_remainingsize : req.upload_chunksize;
				read_ret = read(req.fd, read_buffer, read_chunksize);
				if (read_ret == -1)
				{
					req.unfire(EPOLLIN);
					return (false);
				}
				req.upload_remainingsize -= read_ret;
				req.upload_fd_buff.append(read_buffer, read_ret);
			}

			_parent::cleanup(req.upload_fd);
			nothrow_close(req.upload_fd);
			req.upload_fd = 0;
			
			if (fileExists(req.upload_filename))
			{
				remove(req.upload_filename.c_str());
				res.code = C_NO_CONTENT;
			}
			else
				res.code = C_CREATED;
			
			rename(req.upload_filename_tmp.c_str(), req.upload_filename.c_str());

			if (options.public_root.length())
			{
				res.headers.add("Content-Location: " + sanitizeRelativePath(concatPath(options.public_root, "." + req.trusted_pathname)));
			}
			
			return (true);
		}

};

#endif
