/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 08:00:23 by badam             #+#    #+#             */
/*   Updated: 2022/03/18 08:10:57 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REDIRECT_CPP
# define __REDIRECT_CPP

# include "IMiddleware.hpp"
# include "Response.hpp"
# include "Request.hpp"

static std::string	dummy_redirect(std::string &from)
{
	return from;
}

class Redirect: public IMiddleware
{
	typedef	IMiddleware	_parent;

	public:
		typedef struct	options_s
		{
			std::string	(*convert)(std::string &from);
		}				options_t;
		
		options_t		options;


		Redirect()
		{
			options.convert = dummy_redirect;
		}

		Redirect(options_t opts)
		{
			options = opts;
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			res.headers.set("Location: " + options.convert(req.pathname));
			res.code = C_MOVED_PERMANENTLY;
			
			return (true);
		}

};

#endif
