/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 03:43:21 by badam             #+#    #+#             */
/*   Updated: 2021/08/17 15:31:48 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STATIC_MIDDLEWARE_CPP
# define __STATIC_MIDDLEWARE_CPP

# include "Serve.hpp"
# include "Regex.hpp"
# include "File.hpp"

struct	static_options_t
{
	std::string					root;
	bool						directory_listing;
	std::vector<std::string>	indexes;
};

const static_options_t			staticGetDefaultOptions(void)
{
	static static_options_t	defaultOptions	= {0};

	defaultOptions.root = "./staticfiles";
	defaultOptions.indexes.push_back("index.html");
	defaultOptions.indexes.push_back("index.htm");
	defaultOptions.indexes.push_back("index.php");

	return (defaultOptions);
};

static const static_options_t	getOptions(void)
{
	return staticGetDefaultOptions();
};

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

std::string getIndex(const std::string &path, const static_options_t &options)
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

void	serveStatic(Request &req, Response &res)
{
	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return ;

	std::string				path	= sanitizeRelativePath(req.pathname);
	const static_options_t	options = getOptions();

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
}

#endif
