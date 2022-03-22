/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 01:02:06 by badam             #+#    #+#             */
/*   Updated: 2022/03/23 00:30:18 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERROR_CPP
# define __ERROR_CPP

# include <map>
# include "utils.hpp"
# include "File.hpp"
# include "AEpoll.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "components/page.hpp"
# include "components/utils.hpp"
# include "components/list.hpp"


class Error: public AEpoll
{
	typedef	AEpoll	_parent;

	Log		&_logger;

	protected:
		std::string	_getErrorTitle(Response &)
		{
			return "Test error title";
		}

		std::string	_getErrorMessage(Response &)
		{
			return "Test error message";
		}

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
				open_ret = open(it->second.c_str(), O_NOATIME | O_NOFOLLOW | O_NONBLOCK, O_RDONLY);
				if (open_ret == -1)
					_logger.warn("Failed to open error page: " + it->second);
				return open_ret;
			}
		}

		std::string	generateErrorPage(Response &res)
		{
			return page(
				_getErrorTitle(res),
				title(_getErrorTitle(res)) +
				paragraph(_getErrorMessage(res))
			);
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.errorpage_fd <= 0)
			{
				if (
					(res.code >= 300 && (res.code != 304 && res.code != 305 && res.code != 306) && res.body.length() == 0 && res.response_fd <= 0)
					|| (res.code >= 400)
				)
				{
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
				ssize_t		read_ret = -1;
				char		read_buffer[1024];
				std::string	errorpage;
				
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

				res.body = *replace_all(replace_all(&errorpage, "{title}", _getErrorTitle(res)), "{message}", _getErrorMessage(res));

				_parent::cleanup(res.errorpage_fd);
				nothrow_close(res.errorpage_fd);
				res.errorpage_fd = 0;
				
				return (true);
			}
			
			return (true);
		}

};

#endif
