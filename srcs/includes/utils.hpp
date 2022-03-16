/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 03:49:16 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 06:17:12 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP
# define __UTILS_HPP

# include <time.h>

inline size_t	min(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

size_t	get_elasped(clock_t since)
{
	return (clock() - since) * 1000 / CLOCKS_PER_SEC;
}

#endif