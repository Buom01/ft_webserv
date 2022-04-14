/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 14:41:25 by cbertran          #+#    #+#             */
/*   Updated: 2022/04/03 21:42:30 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSE_HEADER
# define __PARSE_HEADER
# include <iostream>
# include <utility>
# include <string>
# include <algorithm>
# include <vector>
# include <map>
# include "Regex.hpp"

class Header
{
	public:
		typedef std::vector<std::string>					_vectors;
		typedef std::map<std::string, _vectors>				_headers;
		typedef std::map<std::string, _vectors>::iterator	_headers_it;
		typedef std::pair<std::string, _vectors>			_pair;
	private:
		Regex		regex;
		_headers	headersMap;
	public:
		Header() {}
		~Header() {}
	private:
		std::string toLowerCase(std::string _string)
		{
			std::string ret = _string;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return ret;
		}

		std::string	&trim(std::string& str, std::string chars = " \t\n\r\f\v")
		{
			str.erase(0, str.find_first_not_of(chars)); // left
			str.erase(str.find_last_not_of(chars) + 1); // right
			return str;
		}
		
		_headers_it	find(std::string key)
		{
			for (_headers_it check = headersMap.begin(); check != headersMap.end(); check++)
				if (toLowerCase((*check).first) == toLowerCase(key))
					return check;
			return headersMap.end();
		}

		_pair	parse(std::string header)
		{
			_pair vect;
			regex.exec(header, "^([^:]+):(.*)$", GLOBAL_FLAG);
			if (regex.size() == 2)
			{
				vect.first = trim(regex.match()[0].occurence);
				regex.exec(trim(regex.match()[1].occurence), "([^,]+)", GLOBAL_FLAG);
				for (size_t x = 0; x < regex.size(); x++)
					vect.second.push_back(trim(regex.match()[x].occurence));
			}
			return vect;
		}
	public:
		/**
		 *	Remove header if exist
		 * 	Older iterator is invalid
		 *	@param key (std::string) name of header
		 *	@return true if error
		 */
		bool remove(std::string key)
		{
			_headers_it it = headersMap.find(toLowerCase(key));
			if (it != headersMap.end())
				headersMap.erase(toLowerCase(key));
			else
				return true;
			return false;
		}

		/**
		 * 	Add header to container
		 * 	Older iterator is invalid
		 *	@param header (std::string) complete header line
		 *	@return true if error
		 */
		bool add(std::string header)
		{
			_pair	ret = parse(header);
			
			if (ret.first.empty() && ret.second.empty())
				return true;
			if (headersMap.count(ret.first))
				return true;
			headersMap.insert(ret);
			return false;
		}

		/**
		 * 	Set header to container, if exist delete old and replace with new
		 * 	Older iterator is invalid
		 *	@param header (std::string) complete header line
		 *	@return true if error
		 */
		bool set(std::string header)
		{
			_pair	ret = parse(header);
			
			if (ret.first.empty() && ret.second.empty())
				return true;
			if (headersMap.count(ret.first))
				headersMap.erase(ret.first);
			headersMap.insert(ret);
			return false;
		}
	public:
		/**
		 *	Get parsing of HTTP headers
		 *	@return _headers map of headers
		 */
		_headers	map() { return headersMap; }

		/**
		 *	Get every HTTP headers
		 *	@return std::vector<std::string> headers
		 */
		std::vector<std::string> headers()
		{
			std::string					temp;
			std::vector<std::string>	list;
			for (_headers_it it = headersMap.begin(); it != headersMap.end(); it++)
			{
				temp.clear();
				temp = (*it).first + ": ";
				for (_vectors::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end();)
				{
					temp += *it2;
					if (++it2 != (*it).second.end())
						temp += ",";
				}
				list.push_back(temp);
			}
			return list;
		}

		/**
		 * Get value(s) of header
		 * @param key (std::string) of search header
		 * @return (std::string) _vectors of values, empty if not exist
		 */
		_vectors	headerValues(std::string key)
		{
			_headers_it it = find(key);

			if (it != headersMap.end())
				return (*it).second;
			return _vectors();
		}

		/**
		 *	Get specific HTTP header
		 *	@param key (std::string) of search header
		 *	@return (std::string) header
		 */
		std::string	header(std::string key, bool getJustValue = false)
		{
			std::string temp = "";
			_headers_it it = find(key);

			if (it != headersMap.end())
			{
				if (!getJustValue)
					temp = (*it).first + ": ";
				for (_vectors::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end();)
				{
					temp += *it2;
					if (++it2 != (*it).second.end())
						temp += ",";
				}
			}
			return temp;
		}
};

#endif
