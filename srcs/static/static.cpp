/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 03:43:21 by badam             #+#    #+#             */
/*   Updated: 2021/08/11 14:05:22 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STATIC_MIDDLEWARE_CPP
# define __STATIC_MIDDLEWARE_CPP

# include "Serve.hpp"
# include "Regex.hpp"

# define STATIC_ROOT "./staticfiles"

struct  static_options_t
{
    bool                        directory_listing;
    std::vector<std::string>    indexes;
};

static static_options_t    getOptions(Request &req)
{
    static static_options_t defaultOptions = {false};
    defaultOptions.indexes.push_back("index.html");
    defaultOptions.indexes.push_back("index.htm");
    defaultOptions.indexes.push_back("index.php");

    return (defaultOptions);
};


std::string sanitizeInputPath(std::string path)
{
    size_t  i;
    Regex   parent;
    match_t *match;

    // Prepend a slash if missing
    if (path[0] != '/')
        path.insert(0, "/");

    // Remove multiple consecutive slashes
    i = 0;
    while (i < path.size())
    {
        ++i;
        while (i < path.size() && path[i] == '/')
            path.erase(i);
        while (i < path.size() && path[i] != '/')
            ++i;
    }

    // Applying parenting
    while ((match = parent.Match(path, "(^/|/[^/]+/)(\\.+)(/|$)")))
    {
        if (match[2].occurence.size() >= 2)
            path.replace(match[0].start, match[0].width, std::string("/"));
        else
            path.replace(match[0].start, match[0].width, match[1].occurence);
    }

    path.insert(0, STATIC_ROOT);
    return (path);
}

bool    isDirectory(std::string &path)
{
	return (path[path.length() - 1] == '/');
}

bool    serveFile(Response &res, std::string &path)
{
    std::ifstream   fs;

    fs.open(path.c_str(), std::fstream::in | std::ios::binary);
    if (fs && fs.is_open())
    {
        res.body << fs.rdbuf();
        fs.close();
        res.code = C_OK;
        return (true);
    }
    else
        return (false);
}

/*
turn on or off directory listing
default file to answer if the request is a directory
*/
void	serveStatic(Request &req, Response &res)
{
    std::string     path    = sanitizeInputPath(req.pathname);
    if (res.code != C_NOT_IMPLEMENTED)
        return ;

    if (isDirectory(path))
    {
        // try all index then exit
        // else if directory_listing is on, list files
        // else res.code = C_FORBIDDEN
    }
    else if (!serveFile(res, path))
        res.code = C_NOT_FOUND;
}

#endif
