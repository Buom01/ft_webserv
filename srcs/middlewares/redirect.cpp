/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 08:00:23 by badam             #+#    #+#             */
/*   Updated: 2022/04/14 23:28:14 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REDIRECT_CPP
# define __REDIRECT_CPP

# include "IMiddleware.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "utils.hpp"

class Redirect: public IMiddleware
{
	typedef	IMiddleware	_parent;

	public:
		typedef struct	options_s
		{
			http_code_t	code;
			std::string	location;
		}				options_t;
		
		options_t		options;


		Redirect(http_code_t code, std::string &location)
		{
			options.code = code;
			options.location = location;
		}

		Redirect(options_t opts)
		{
			options = opts;
		}

		virtual ~Redirect()
		{}

	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.code != C_NOT_IMPLEMENTED)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			
			std::string	newLocation(options.location);

			replace_all(&newLocation, "$request_uri", req.pathname);
			res.headers.set("Location: " + newLocation);
			res.code = options.code;

			std::cout << "Done" << std::endl;
			
			return (true);
		}

};

#endif
