/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remover.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/04/12 04:48:43 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REMOVER_CPP
# define __REMOVER_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"
# include "IMiddleware.hpp"
# include "utils.hpp"
# include "File.hpp"

class Remover: public IMiddleware
{
	typedef	IMiddleware	_parent;

	public:
		typedef struct	options_s
		{
			std::string	destination;
			std::string	public_root;
		}				options_t;
		
		options_t		options;

		Remover(std::string destination, std::string public_root = "")
		{
			options.destination = destination;
			options.public_root = public_root;
		}

		virtual ~Remover()
		{}
	
	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			if (req.finish())
				return (true);

			std::string	remove_file_path	= concatPath(options.destination, "." + req.trusted_pathname);

			if (fileExists(remove_file_path))
				remove(remove_file_path.c_str());
				
			res.code = C_NO_CONTENT;
			
			return (true);
		}

};

#endif
