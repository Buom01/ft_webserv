#ifndef __COMPONENTS_LIST_HPP
# define __COMPONENTS_LIST_HPP
# include "builtin.hpp"
# include "comp_utils.hpp"

template<class List, class ItemType>
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
