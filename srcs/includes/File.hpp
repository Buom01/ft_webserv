/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 12:59:44 by badam             #+#    #+#             */
/*   Updated: 2022/03/13 01:03:36 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __File_HPP
# define __File_HPP
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>

/*
** Following function limit parenting to root and simplify path as much as possible.
** It keeps trailing slash as is.
*/
std::string sanitizeRelativePath(std::string path)
{
	size_t	i;
	Regex	parent;
	match_t	*match;

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
	while ((match = parent.exec(path, "(^/|/[^/]+/)(\\.+)(/|$)", GLOBAL_FLAG)))
	{
		if (match[2].occurence.size() >= 2)
			path.replace(match[0].start, match[0].width, std::string("/"));
		else
			path.replace(match[0].start, match[0].width, match[1].occurence);
	}

	return (path);
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
	return (path[path.length() - 1] == '/');
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

std::vector<std::string>	listFile(const std::string &path)
{
	std::vector<std::string>	files;
	DIR							*dir;
	struct dirent				*ent;

	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
			files.push_back(path + ent->d_name);
		closedir (dir);
	}

	return (files);
}

#endif