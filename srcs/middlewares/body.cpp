#ifndef __BODY_CPP
# define __BODY_CPP
# define LF "\n"
# define CRLF "\r\n"
# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "AEpoll.hpp"
# include "utils.hpp"
# include "http.hpp"
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
					req.body_boundary_end_webkit.assign(req.body_boundary);
					req.body_boundary_end_webkit.append("--");
					req.body_boundary_end.assign("--");
					req.body_boundary_end.append(req.body_boundary);
					req.body_boundary_end.append("--");
					break;
				}
			}
			return (true);
		}
	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			if (req.finish())
				return (true);
			if (req.body.empty()
				&& req.method != M_POST
				&& req.method != M_PUT
				&& req.method != M_ALL)
			return (true);
			if (req.body_read_is_finished)
				return (true);
			if (!req.body_header_parsed)
			{
				_get_contentlength(req);
				_get_bondary(req);
				_get_transferEncoding(req, res);
				req.body_header_parsed = true;
			}
			if (!req.await(EPOLLIN))
				return (false);

			if (req.body_chuncked == true)
			{
				std::string	line("");
				ssize_t		chunkSize(0), readSize(0);
				bool		isChunk(false);

				while (get_next_line_string(req.fd, line, req.buff))
				{
					if (!isChunk)
					{
						chunkSize = 0;
						readSize = 0;
						std::stringstream	sstream(line);
						sstream >> std::hex >> chunkSize;
						if (chunkSize <= 0)
						{
							req.body_read_is_finished = true;
							break;
						}
						isChunk = true;
					}
					else
					{
						req.body.append(line);
						req.body.append(CRLF);
						readSize += line.size();
						if (readSize >= chunkSize)
							isChunk = false;
					}
				}
			}
			else
			{
				std::string	line("");
				ssize_t		body_size(0);
				
				while (get_next_line_string(req.fd, line, req.buff))
				{
					req.body.append(line);
					req.body.append(CRLF);
					if (!req.body_boundary_end.empty())
					{
						if (line.compare(req.body_boundary_end) == 0
							|| line.compare(req.body_boundary_end_webkit) == 0)
						{
							req.body_read_is_finished = true;
							break;
						}
					}
					else
					{
						body_size += line.size();
						if (body_size >= req.body_length)
						{
							req.body_read_is_finished = true;
							break;
						}
					}
				}
			}
			return (req.body_read_is_finished);
		}
};

#endif
