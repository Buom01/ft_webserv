/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 12:59:44 by badam             #+#    #+#             */
/*   Updated: 2022/04/17 02:42:40 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __File_HPP
# define __File_HPP
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>

void	nothrow_close(int fd)
{
	try
	{
		close(fd);
	}
	catch(...)
	{}
}

/*
** Following function limit parenting to root and simplify path as much as possible.
** It keeps trailing slash as is.
*/
std::string sanitizeRelativePath(std::string path)
{
	size_t	i;
	Regex	parent;
	//match_t *match

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
	while (parent.exec(path, "(^/|/[^/]+/)(\\.+)(/|$)"))
	{
		if (parent.size() <= 0)
			break;
		if (parent.match()[1].occurence.size() >= 2)
			path.replace(parent.match()[0].group.start, parent.match()[0].group.width, std::string("/"));
		else
			path.replace(parent.match()[0].group.start, parent.match()[0].group.width, parent.match()[0].occurence);
	}
	return (path);
}

std::string	relativeToAbsoluteDir(const std::string &path)
{
	char		absPath[PATH_MAX];
	std::string	plainPath;
	size_t		lastSlash;

	if (realpath(path.c_str(), absPath))
		plainPath = absPath;
	else
		plainPath = path;

	lastSlash = plainPath.find_last_of('/');
	if (lastSlash == std::string::npos)
		return (plainPath);
	else
		return (plainPath.substr(0, lastSlash));
}

std::string	concatPath(const std::string &root, const std::string &path)
{
	if (!path.size())
		return ("");
	if (path.at(0) == '/')
		return (path);
	else
	{
		if (root.size() && root.at(root.size() - 1) == '/')
			return (root + path);
		else
			return (root + '/' + path);
	}
}

bool	hasPermissions(const std::string &path)
{
	return (access(path.c_str(), F_OK) == 0);
}

bool	hasReadPermissions(const std::string &path)
{
	return (access(path.c_str(), R_OK) == 0);
}

bool	hasExecPermissions(const std::string &path)
{
	return (access(path.c_str(), R_OK | X_OK) == 0);
}

bool	isDirectory(const std::string &path)
{
	return (!path.length() || (path[path.length() - 1] == '/'));
}

bool	directoryExists(const std::string &path)
{
	struct stat sb;

	return (!stat(path.c_str(), &sb) && S_ISDIR(sb.st_mode));
}

bool	fileExists(const std::string &path)
{
	struct stat	sb;

	return (!stat(path.c_str(), &sb) && S_ISREG(sb.st_mode));
}


std::string	getExtension(std::string path)
{
	size_t		lastDot = path.find_last_of(".");

	if (lastDot == std::string::npos)
		return "";
	else
		return (path.substr(lastDot + 1));
}

/*
** It also allow us to know if the FD belongs to a file
*/
off_t	fdFileSize(int fd)
{
	struct stat	sb;

	if (fstat(fd, &sb) < 0)
		return (-1);
	else
		return (sb.st_size);
}

std::vector<std::string>	listFile(const std::string &path)
{
	std::vector<std::string>	files;
	DIR							*dir;
	struct dirent				*ent;

	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
			files.push_back(ent->d_name);
		closedir (dir);
	}

	return (files);
}

#endif
