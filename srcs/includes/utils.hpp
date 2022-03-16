/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 03:49:16 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 17:44:49 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP
# define __UTILS_HPP

# include <time.h>

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

#endif