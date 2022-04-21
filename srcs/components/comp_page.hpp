#ifndef __COMPONENTS_PAGE_HPP
# define __COMPONENTS_PAGE_HPP
# include "builtin.hpp"
# include "comp_utils.hpp"

std::string	page(const std::string &title, const std::string &content, const std::string lang = "en", const std::string metas = "")
{
	return (
	"<!DOCTYPE html>\n"
	"<html lang="+lang+">"
		"<head>"
			"<title>"+escape_html(title)+"</title>"
			+metas+
		"</head>"
		"<body>"
			+content+
		"</body>"
	"</html>"
	);
}

#endif