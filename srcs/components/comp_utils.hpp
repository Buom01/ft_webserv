#ifndef __COMPONENTS_UTILS_HPP
# define __COMPONENTS_UTILS_HPP
# include "builtin.hpp"

std::string	escape_html(const std::string &data)
{
    std::string buff;
	size_t		i		= 0;

    buff.reserve(data.size() * 1.05);

	while (i < data.size())
	{
        switch(data.at(i))
		{
            case '&':  buff.append("&amp;");		break;
            case '<':  buff.append("&lt;");			break;
            case '>':  buff.append("&gt;");			break;
            case '\"': buff.append("&quot;");		break;
            case '\'': buff.append("&apos;");		break;
            default:   buff += (data.at(i));		break;
        }
		++i;
	}
	
	return (buff);
}

std::string title(const std::string &title, int level = 1)
{
	std::string			html;
	std::stringstream	tag;

	tag << "h";
	tag << level;

	html = "<" + tag.str() + ">";
	html += escape_html(title);
	html += "</" + tag.str() + ">";

	return (html);
}

std::string	paragraph(const std::string &content)
{
	std::string ret("");

	ret.append("<p>");
	ret.append(escape_html(content));
	ret.append("</p>");
	return (ret);
}

std::string	link(std::string &href, std::string &text)
{
	std::string ret("");

	ret.append("<a href=\"");
	ret.append(escape_html(href));
	ret.append("\">");
	ret.append(escape_html(text));
	ret.append("</a>");
	return (ret);
}

std::string linkify(std::string url)
{
	return link(url, url);
}

#endif