#ifndef __BODY_CPP
# define __BODY_CPP

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
		
	public:
		
		Body(Log &logger):
			_parent(logger),
			_logger(logger)
		{}

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
				return (false);
			if (header_values_it != header_values.end())
			{
				std::stringstream	sstream(*header_values_it);
				sstream >> req.body_length;
			}
			return (true);
		}

		bool	_get_bondary(Request &req, Response &res)
		{
			size_t _pos(0);
			std::vector<std::string>					header_values;
			std::vector<std::string>::const_iterator	header_values_it;

			header_values = req.headers.headerValues("content-type");
			header_values_it = header_values.begin();
			if (header_values.begin() == header_values.end())
				return (false);
			for (header_values_it it = header_values.begin(); it != header_values.end(); it++)
			{
				_pos = (*it).find("boundary=");
				if (_pos != std::string::n_pos)
				{
					req.body_boundary = (*it);
					req.body_boundary.substring(0, _pos);
					break;
				}		
			}
			return (true);
		}
		
	public:
		bool	operator()(Request &req, Response &res)
		{
			std::string	line();
			size_t	read_chunksize;
			char	read_buffer[req.upload_chunksize];
			ssize_t	read_ret							= -1;
			ssize_t	write_ret							= -1;
			
			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			if (req.finish())
				return (true);
			if (req.body.empty() && (!_get_contentlength(req, res) || !_get_bondary(req, res)))
				return (true);

			if (!req.await(EPOLLIN))
				return (false);
		
			size_t _size = std::strlen(req.buff);
			if (_size)
			{
				body.append(req.buff, _size);
				req.buff[0] = '\0';
			}
			
			while (req.body_remainingsize > 0)
			{
				read_chunksize = req.body_remainingsize < req.body_chunksize ? req.body_remainingsize : req.body_chunksize;
				read_ret = read(req.fd, read_buffer, read_chunksize);
				if (read_ret == -1)
				{
					req.unfire(EPOLLIN);
					return (false);
				}
				req.body_remainingsize -= read_ret;
				line.append(read_buffer, read_ret);
				
				if (req.body_boundary)
				{

				}
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
