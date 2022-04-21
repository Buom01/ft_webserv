#ifndef __COMPONENTS_HPP
# define __COMPONENTS_HPP
# include "builtin.hpp"

std::string	escape_html(const std::string &);
std::string title(const std::string &, int = 1);
std::string	paragraph(const std::string &);
std::string	link(std::string &, std::string &);
std::string linkify(std::string);

std::string	page(
	const std::string &,
	const std::string &,
	const std::string = "en",
	const std::string = ""
);

template<class List, class ItemType>
std::string list(List &list, std::string (*convert)(ItemType item) = escape_html)
{
	typename List::const_iterator	it		= list.begin();
	std::string						html("<ul>");

	while (it != list.end())
	{
		html += "<li>"+convert(*it)+"</li>";
		++it;
	}
	html += "</ul>";
	return (html);
}

#endif
