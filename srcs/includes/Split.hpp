/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:53:14 by cbertran          #+#    #+#             */
/*   Updated: 2022/02/13 23:39:35 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SPLIT_HPP
# define __SPLIT_HPP
# include <string>
# include <vector>
# include <utility>

typedef std::vector<std::string>	s_split;

/**
 * 	Split string to token with delimiter(s), return s_split of element(s)
 * 	@param str: string to split
 * 	@param delimiter: string of delimiter(s)
 */
s_split split(std::string str, std::string delimiter)
{
	s_split data;
	int start = 0;
	int end = str.find(delimiter);
	while (end != -1)
    {
        data.push_back(str.substr(start, end - start));
		start = end + delimiter.size();
		end = str.find(delimiter, start);
	}
    data.push_back(str.substr(start, end - start));
    return data;
}

#endif
