/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:37:00 by badam             #+#    #+#             */
/*   Updated: 2022/03/21 01:00:14 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COMPONENTS_LIST_HPP
# define __COMPONENTS_LIST_HPP

# include <string>
# include "components/utils.hpp"

template<class List, class ItemType = std::string>
std::string list(List &list, std::string (*convert)(ItemType item) = escape_html)
{
	typename List::const_iterator	it		= list.begin();
	std::string						html;

	html = "<ul>";
	while (it != list.end())
	{
		html += "<li>"+convert(*it)+"</li>";
		++it;
	}
	html += "</ul>";

	return (html);
}

#endif