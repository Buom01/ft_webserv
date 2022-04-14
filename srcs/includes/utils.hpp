/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 03:49:16 by badam             #+#    #+#             */
/*   Updated: 2022/04/14 01:12:35 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP
# define __UTILS_HPP

# include <time.h>
# include <string>
# include <sstream>
# include "http.hpp"

inline size_t	min(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

struct timespec	get_time()
{
	struct timespec	time;
	clock_gettime(CLOCK_MONOTONIC,&time);

	return (time);
}

int64_t difftimespec_ns(const struct timespec after, const struct timespec before)
{
    return static_cast<int64_t>(after.tv_sec - before.tv_sec) * 1000000000
         + static_cast<int64_t>(after.tv_nsec - before.tv_nsec);
}

float difftimespec_ms(const struct timespec after, const struct timespec before)
{
    return static_cast<float>(after.tv_sec - before.tv_sec) * 1000
         + static_cast<float>(after.tv_nsec - before.tv_nsec) / 1000000;
}

int64_t	get_elasped_ns(struct timespec since)
{
	return difftimespec_ns(get_time(), since);
}

float	get_elasped_ms(struct timespec since)
{
	return difftimespec_ms(get_time(), since);
}

std::string *replace_all(std::string *str, const std::string& from, const std::string& to)
{
    size_t	start_pos   = 0;

    while((start_pos = str->find(from, start_pos)) != std::string::npos)
	{
        str->replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
	
    return str;
}

std::string bind_to_string(server_bind_t &bind)
{
    std::stringstream    sstr;

    sstr << bind.host;
    sstr << ":";
    sstr << bind.port;

    return (sstr.str());
}

method_t method(Parse::s_allow allow)
{
	method_t ret = M_UNKNOWN;
	if (allow.GET)
		ret = static_cast<method_t>(ret | M_GET);
	if (allow.HEAD)
		ret = static_cast<method_t>(ret | M_HEAD);
	if (allow.POST)
		ret = static_cast<method_t>(ret | M_POST);
	if (allow.PUT)
		ret = static_cast<method_t>(ret | M_PUT);
	if (allow.DELETE)
		ret = static_cast<method_t>(ret | M_DELETE);
	if (allow.CONNECT)
		ret = static_cast<method_t>(ret | M_CONNECT);
	if (allow.OPTIONS)
		ret = static_cast<method_t>(ret | M_OPTIONS);
	if (allow.TRACE)
		ret = static_cast<method_t>(ret | M_TRACE);
	if (allow.ALL)
		ret = static_cast<method_t>(ret | M_ALL);
	return ret;
}

#endif