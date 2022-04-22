#ifndef __RESPONSE_HPP
# define __RESPONSE_HPP
# include "builtin.hpp"
# include "lib.hpp"
# include "Log.hpp"
# include "Header.hpp"

class Response
{
	public:
		int						fd;
		Log						&logger;
		const std::exception	*error;
		
		std::string				used_file;
		http_code_t				code;
		Header					headers;
		std::string				headers_buff;
		bool					headers_sent;
		int						response_fd;
		int 					response_fd_header_size;
		std::string				response_fd_buff;
		int						errorpage_fd;
		std::string				errorpage;
		std::string				body;
		bool					sent;

		Response(int connection, Log &_logger) :
			fd(connection),
			logger(_logger),
			error(NULL),
			used_file(""),
			code(C_NOT_IMPLEMENTED),
			headers(),
			headers_buff(""),
			headers_sent(false),
			response_fd(0),
			response_fd_header_size(0),
			response_fd_buff(""),
			errorpage_fd(0),
			errorpage(""),
			body(""),
			sent(false)
		{}

		virtual ~Response()
		{}

		void	reset()
		{
			error = NULL;
			used_file = "";
			code = C_NOT_IMPLEMENTED;
			headers.reset();
			headers_buff = "";
			headers_sent = false;
			response_fd = 0;
			response_fd_header_size = 0;
			response_fd_buff = "";
			errorpage_fd = 0;
			errorpage = "";
			body = "";
			sent = false;
		}
};

#endif
