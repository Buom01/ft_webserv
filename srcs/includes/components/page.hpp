/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 21:30:34 by badam             #+#    #+#             */
/*   Updated: 2022/03/21 00:42:36 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COMPONENTS_PAGE_HPP
# define __COMPONENTS_PAGE_HPP

# include <string>
# include "components/utils.hpp"

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