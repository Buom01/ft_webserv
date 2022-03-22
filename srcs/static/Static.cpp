/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Static.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 03:43:21 by badam             #+#    #+#             */
/*   Updated: 2022/03/21 16:35:48 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STATIC_MIDDLEWARE_CPP
# define __STATIC_MIDDLEWARE_CPP

# include "IMiddleware.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Regex.hpp"
# include "File.hpp"
# include "AEpoll.hpp"
# include "components/page.hpp"
# include "components/utils.hpp"
# include "components/list.hpp"


class Static: public IMiddleware
{
	typedef	IMiddleware	_parent;

	public:
		typedef struct	options_s
		{
			std::string					root;
			bool						directory_listing;
			std::vector<std::string>	indexes;
		}				options_t;
		
		options_t		options;


		Static()
		{
			options.root = "./";
			options.directory_listing = false;
			options.indexes.push_back("index.html");
			options.indexes.push_back("index.htm");
			options.indexes.push_back("index.php");
		}

		Static(options_t opts)
		{
			options = opts;
		}

	protected:
		bool	serveDirectory(Response &res, const std::string &server_path, const std::string &client_path)
		{
			if (!hasReadPermissions(server_path))
				return (false);

			std::vector<std::string>	files	= listFile(server_path);

			res.headers.set("Content-Type: text/html");
			res.code = C_OK;
			res.body = page(
				"List of file in " + client_path,
				title("List of files in " + client_path) +
				list(files, linkify)
			);

			return (true);

		}

		void	serveFile(Response &res, const std::string &path)
		{
			int	fd = open(path.c_str(), O_NOATIME | O_NOFOLLOW | O_NONBLOCK, O_RDONLY);

			if (fd > 0)
			{
				res.response_fd = fd;
				res.code = C_OK;
				res.used_file = path;
			}
			else
				res.code = C_FORBIDDEN;
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
			if (res.response_fd > 0 || res.body.length() > 0)
				return (true);
			if (req.finish())
				return (true);
			if (req.timeout())
			{
				res.code = C_REQUEST_TIMEOUT;
				return (true);
			}

			std::string				path	= req.trusted_pathname;

			path.insert(0, options.root);

			if (isDirectory(path))
			{
				if (directoryExists(path))
				{
					std::string index = getIndex(path, options);

					if (index.size() > 0)
						serveFile(res, index);
					else if (options.directory_listing)
					{
						if (!serveDirectory(res, path, req.trusted_pathname))
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
				serveFile(res, path);
			else
				res.code = C_NOT_FOUND;
			
			return (true);
		}

};

#endif
