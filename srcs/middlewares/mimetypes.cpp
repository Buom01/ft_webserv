/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimetypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 08:00:23 by badam             #+#    #+#             */
/*   Updated: 2022/03/19 06:20:20 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MIMETYPES_CPP
# define __MIMETYPES_CPP

# include <map>
# include "IMiddleware.hpp"
# include "Response.hpp"
# include "Request.hpp"

class Mimetypes: public IMiddleware
{
	typedef	IMiddleware							_parent;
	typedef	std::map<std::string, std::string>	mimetypes_t;

	public:
		typedef struct	options_s
		{
			std::string	fallback;
			mimetypes_t	mimetypes;
		}				options_t;
		
		options_t		options;


		Mimetypes()
		{
			options.fallback = "application/octet-stream";
		}

		Mimetypes(options_t opts)
		{
			options = opts;
		}

		void	add(std::string ext, std::string mimetype)
		{
			options.mimetypes.insert(std::pair<std::string, std::string>(ext, mimetype));
		}

		std::string	getMimetype(std::string &path)
		{
			mimetypes_t::const_iterator	it;

			if (isDirectory(path))
				return (options.fallback);
			
			it = options.mimetypes.find(getExtension(path));

			if (it == options.mimetypes.end())
				return (options.fallback);
			else
				return it->second;
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			std::string	&path	= res.used_file.length() ? res.used_file : req.trusted_pathname;

			res.headers.add("Content-Type: " + getMimetype(path));
			
			return (true);
		}

};

#endif