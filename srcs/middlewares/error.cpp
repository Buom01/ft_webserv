/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 01:02:06 by badam             #+#    #+#             */
/*   Updated: 2022/04/13 22:43:17 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERROR_CPP
# define __ERROR_CPP

# include <map>
# include <string>
# include <sstream>
# include "utils.hpp"
# include "File.hpp"
# include "AEpoll.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "components/page.hpp"
# include "components/utils.hpp"
# include "components/list.hpp"


static std::string	getErrorTitle(Response &res)
{
	std::stringstream	title;

	switch (res.code)
	{
		case C_MULTIPLE_CHOICE:
			return ("Multiple choice");
		case C_MOVED_PERMANENTLY:
			return ("Moved permanently");
		case C_FOUND:
			return ("Found");
		case C_SEE_OTHER:
			return ("See other");
		case C_NOT_MODIFIED:
			return ("Not modified");
		case C_USE_PROXY:
			return ("Use proxy");
		case C_TEMPORARY_REDIRECT:
			return ("Temporary redirect");
		case C_PERMANENT_REDIRECT:
			return ("Permanent redirect");

		case C_BAD_REQUEST:
			return ("Bad request");
		case C_UNAUTHORIZED:
			return ("Unauthorized");
		case C_PAYMENT_REQUIRED:
			return ("Payment required");
		case C_FORBIDDEN:
			return ("Forbidden");
		case C_NOT_FOUND:
			return ("Not found");
		case C_METHOD_NOT_ALLOWED:
			return ("Method not allowed");
		case C_NOT_ACCEPTABLE:
			return ("Not acceptable");
		case C_PROXY_AUTHENTICATION_REQUIRED:
			return ("Proxy authentication required");
		case C_REQUEST_TIMEOUT:
			return ("Request timeout");
		case C_CONFLICT:
			return ("Conflict");
		case C_GONE:
			return ("Gone");
		case C_LENGTH_REQUIRED:
			return ("Length required");
		case C_PRECONDITION_FAILED:
			return ("Precondition failed");
		case C_REQUEST_ENTITY_TOO_LARGE:
			return ("Request entity too large");
		case C_REQUEST_URI_TOO_LONG:
			return ("Request URI too long");
		case C_UNSUPPORTED_MEDIA_TYPE:
			return ("Unsuported media type");
		case C_REQUEST_RANGE_NOT_SATISFIABLE:
			return ("Request range not satisfiable");
		case C_EXPECTATION_FAILED:
			return ("Expectation failed");
		case C_IM_A_TEAPOT:
			return ("I'm a teapot");

		case C_INTERNAL_SERVER_ERROR:
			return ("Internal server error");
		case C_NOT_IMPLEMENTED:
			return ("Not implemented");
		case C_BAD_GATEWAY:
			return ("Bad gateway");
		case C_SERVICE_UNAVAILABLE:
			return ("Service unavailable");
		case C_GATEWAY_TIMEOUT:
			return ("Gateway timeout");
		case C_HTTP_VERSION_NOT_SUPPORTED:
			return ("HTTP version not supported");

		default:
			title << "Error ";
			title << static_cast<int>(res.code);
			return (title.str());
	}
}

static std::string	getErrorMessage(Response &res)
{
	std::stringstream	message;

	switch (res.code)
	{
		case C_MULTIPLE_CHOICE:
			return ("The request has more than one possible response.");
		case C_MOVED_PERMANENTLY:
			return ("The URL of the requested resource has been changed permanently."); // @TODO: DETAILS WANTED
		case C_FOUND:
			return ("The URI of requested resource has been changed temporarily.");  // @TODO: DETAILS WANTED
		case C_SEE_OTHER:
			return ("The requested resource could be got at another URI with a GET request."); // @TODO: DETAILS WANTED
		case C_NOT_MODIFIED:
			return ("The cached file is still valid.");
		case C_USE_PROXY:
			return ("The requested response must be accessed through a proxy.");
		case C_TEMPORARY_REDIRECT:
			return ("You are being redirected to a temporary URI."); // @TODO: DETAILS WANTED
		case C_PERMANENT_REDIRECT:
			return ("You are being redirected to a permanent URI."); // @TODO: DETAILS WANTED

		case C_BAD_REQUEST:
			return ("The request is perceived to be malformed.");
		case C_UNAUTHORIZED:
			return ("This request must be made while have been authenticated to get the requested response.");
		case C_PAYMENT_REQUIRED:
			return ("A payment is required to fulfill this request.");
		case C_FORBIDDEN:
			return ("You does not have access rights to the content.");
		case C_NOT_FOUND:
			return ("The requested resource cannot be found.");
		case C_METHOD_NOT_ALLOWED:
			return ("The request method cannot be used on that resource.");
		case C_NOT_ACCEPTABLE:
			return ("There are no content that conforms to the requested criteria.");
		case C_PROXY_AUTHENTICATION_REQUIRED:
			return ("This request must be made while have been authenticated through a proxy to get the requested response.");
		case C_REQUEST_TIMEOUT:
			return ("Since no response could be given in time, the server would like to shut down this connection.");
		case C_CONFLICT:
			return ("The request conflicts with the current state of the server.");
		case C_GONE:
			return ("The requested content has been permanently deleted from server.");
		case C_LENGTH_REQUIRED:
			return ("The Content-Length header field is not defined while the server requires it.");
		case C_PRECONDITION_FAILED:
			return ("Indicated preconditions are not meet by the server.");
		case C_REQUEST_ENTITY_TOO_LARGE:
			return ("Request entity is larger than limits defined by server.");
		case C_REQUEST_URI_TOO_LONG:
			return ("The URI requested by the client is longer than the server is willing to interpret.");
		case C_UNSUPPORTED_MEDIA_TYPE:
			return ("The media format of the requested data is not supported.");
		case C_REQUEST_RANGE_NOT_SATISFIABLE:
			return ("The range specified by the Range header field in the request cannot be fulfilled.");
		case C_EXPECTATION_FAILED:
			return ("The expectation indicated by the Expect request header field cannot be met by the server.");
		case C_IM_A_TEAPOT:
			return ("Refuse the attempt to brew coffee with a teapot.");

		case C_INTERNAL_SERVER_ERROR:
			return ("The server has encountered a situation it does not know how to handle."); // @TODO: DETAILS WANTED
		case C_NOT_IMPLEMENTED:
			return ("The request method is not supported and cannot be handled.");
		case C_BAD_GATEWAY:
			return ("Got an invalid response while working as a gateway.");
		case C_SERVICE_UNAVAILABLE:
			return ("Not ready to handle the request.");
		case C_GATEWAY_TIMEOUT:
			return ("Cannot get a response in time while working as a gateway.");
		case C_HTTP_VERSION_NOT_SUPPORTED:
			return ("The HTTP version used in the request is not supported.");

		default:
			message << "An HTTP error happened; code ";
			message << static_cast<int>(res.code);
			return (message.str());
	}
}


class Error: public AEpoll
{
	typedef	AEpoll	_parent;

	Log		&_logger;

	public:
		typedef	std::map<int, std::string>	errorpages_t;
		
		typedef struct	options_s
		{
			errorpages_t	errorpages;
		}				options_t;
		
		options_t		options;


		Error(Log &logger):
			_parent(logger),
			_logger(logger)
		{}

		Error(options_t opts, Log &logger):
			_parent(logger),
			_logger(logger)
		{
			options = opts;
		}

		virtual ~Error()
		{}

		void	add(int error, std::string page)
		{
			options.errorpages.insert(std::pair<int, std::string>(error, page));
		}

		int	getErrorpageFD(int code)
		{
			errorpages_t::const_iterator	it;
			int								open_ret;

			it = options.errorpages.find(code);

			if (it == options.errorpages.end())
				return (-1);
			else
			{
				open_ret = open(it->second.c_str(), O_NONBLOCK | O_RDONLY);
				if (open_ret == -1)
					_logger.warn("Failed to open error page: " + it->second);
				return open_ret;
			}
		}

		std::string	generateErrorPage(Response &res)
		{
			std::stringstream	code_str;

			code_str << res.code;

			return page(
				getErrorTitle(res),
				title(code_str.str() + " - " + getErrorTitle(res)) +
				paragraph(getErrorMessage(res))
			);
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
				
			if (res.errorpage_fd <= 0)
			{
				if (
					(res.code >= 300 && (res.code != 304 && res.code != 305 && res.code != 306) && res.body.length() == 0 && res.response_fd <= 0)
					|| (res.code >= 400)
					|| res.code == C_UNKNOWN
				)
				{
					if (res.code == C_UNKNOWN)
						res.code = C_NOT_IMPLEMENTED;
					if (res.response_fd > 0)
					{
						nothrow_close(res.response_fd);
						res.response_fd = -1;
					}
					res.headers.set("Content-Type: text/html");
					
					res.errorpage_fd = getErrorpageFD(res.code);

					if (res.errorpage_fd <= 0)
					{
						res.body = generateErrorPage(res);
						return (true);
					}
					else
					{
						res.body = "";
						return (false);
					}
				}
			}
			else
			{
				ssize_t				read_ret 			= -1;
				char				read_buffer[1024];
				std::string			errorpage;
				std::stringstream	code_str;
				
				if (req.finish())
				{
					if (_parent::has(res.errorpage_fd))
					{
						_parent::cleanup(res.errorpage_fd);
						nothrow_close(res.errorpage_fd);
					}
					return (true);
				}
					
				if (!_parent::has(res.errorpage_fd))
					_parent::setup(res.errorpage_fd, ET_BODY);
				if (!_parent::await(res.errorpage_fd, EPOLLIN))
					return (false);
				
				while (read_ret != 0 )
				{
					read_ret = read(res.errorpage_fd, read_buffer, 1024);
					if (read_ret == -1)
					{
						_parent::clear_events(res.errorpage_fd, EPOLLIN);
						return (false);
					}
					errorpage.append(read_buffer, read_ret);
				}

				code_str << res.code;

				res.body = *replace_all(replace_all(replace_all(&errorpage, "{code}", code_str.str()), "{title}", getErrorTitle(res)), "{message}", getErrorMessage(res));

				_parent::cleanup(res.errorpage_fd);
				nothrow_close(res.errorpage_fd);
				res.errorpage_fd = 0;
				
				return (true);
			}
			
			return (true);
		}

};

#endif
