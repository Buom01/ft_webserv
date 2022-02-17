/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Static.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 03:43:21 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 23:58:19 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STATIC_MIDDLEWARE_CPP
# define __STATIC_MIDDLEWARE_CPP

# include "Response.hpp"
# include "Request.hpp"
# include "Regex.hpp"
# include "File.hpp"
# include "AEpoll.hpp"

class Static: public AEpoll
{
	typedef	AEpoll	_parent;

	public:
		typedef struct	options_s
		{
			std::string					root;
			bool						directory_listing;
			std::vector<std::string>	indexes;
		}				options_t;
		
		options_t		options;


		Static(Log &logger): _parent(logger)
		{
			options.root = "./";
			options.directory_listing = false;
			options.indexes.push_back("index.html");
			options.indexes.push_back("index.htm");
			options.indexes.push_back("index.php");
		}

		Static(Log &logger, options_t opts): _parent(logger)
		{
			options = opts;
		}

	protected:
		bool	serveDirectory(Response &res, const std::string &path)
		{
			if (!hasReadPermissions(path))
				return (false);

			// std::vector<std::string>	files	= listFile(path);
			// write res.body here with template
			// res.code = C_OK;

			// return (true);

			return (false);
		}

		bool	serveFile(Response &res, const std::string &path)
		{
			std::ifstream	fs;

			fs.open(path.c_str(), std::fstream::in | std::ios::binary);
			if (!fs || !fs.is_open())
				return (false);

			res.body << fs.rdbuf();
			fs.close();
			res.code = C_OK;

			return (true);
		}

		std::string getIndex(const std::string &path, const options_t &options)
		{
			std::vector<std::string>::const_iterator	it			= options.indexes.begin();
			std::string									candidate;

			while (it != options.indexes.end())
			{
				candidate = path + *it;
				if (fileExists(candidate))
					return (candidate);
				++it;
			}

			return ("");
		}

		void	redirect(Response &res, std::string path)
		{
			res.headers.set("Location: " + path);
			res.code = C_MOVED_PERMANENTLY;
		}

	public:
		bool	operator()(Request &req, Response &res)
		{
			if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
				return (true);

			std::string				path	= sanitizeRelativePath(req.pathname);

			path.insert(0, options.root);

			if (isDirectory(path))
			{
				if (directoryExists(path))
				{
					std::string index = getIndex(path, options);

					if (index.size() > 0)
					{
						if (!serveFile(res, index))
							res.code = C_FORBIDDEN;
					}
					else if (options.directory_listing)
					{
						if (!serveDirectory(res, path))
							res.code = C_FORBIDDEN;
					}
					else
						res.code = C_FORBIDDEN;
				}
				else
					res.code = C_NOT_FOUND;
			}
			else if (directoryExists(path + "/"))
				redirect(res, req.pathname + "/");
			else if (fileExists(path))
			{
				if (!serveFile(res, path))
					res.code = C_FORBIDDEN;
			}
			else
				res.code = C_NOT_FOUND;
			
			return (true);
		}

};

#endif
