/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 14:41:25 by cbertran          #+#    #+#             */
/*   Updated: 2021/07/12 15:57:37 by cbertran         ###   ########.fr       */
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

/** Types of headers
 *	Host
 *	Accept-Charsets
 *	//Accept-Language
 *	Allow
 *	Authorization
 * 	
 *	Content-Language
 *	Content-Length
 *	Content-Location
 *	Content-Type
 */

class Header
{
	private:
		typedef std::multimap<std::string, std::string> _Container;
	private:
		Regex			_regex;
		_Container		_header;
	private:
		std::string toLowerCase(std::string const _string)
		{
			std::string ret = _string;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return ret;
		}
	public:
		Header() {}
		~Header() {}

		/**
		 * 	Get map of every header.
		 */
		_Container GetEveryHeader()
		{ return _header; }
		

		/**
		 * 	Return well formated string represent HTTP header
		 * 	@param it: iterator of multimap
		 */
		std::string HTTPheader(_Container::iterator &it)
		{
			std::vector<std::string> HTTP = GetHeader(toLowerCase((*it).first));
			std::string ret = toLowerCase((*it).first);
			ret += ": ";
			for (std::vector<std::string>::iterator it2 = HTTP.begin(); it2 != HTTP.end(); ++it2)
				ret += *it2;
			return ret;
		}
		
		/**
		 * 	Return well formated string represent HTTP header
		 * 	@param search: case-insensitive name of header
		 */
		std::string HTTPheader(std::string const search)
		{
			std::vector<std::string> HTTP = GetHeader(toLowerCase(search));
			std::string ret = toLowerCase(search);
			ret += ": ";
			for (std::vector<std::string>::iterator it2 = HTTP.begin(); it2 != HTTP.end(); ++it2)
				ret += *it2;
			return ret;
		}
		
		/**
		 * 	Get value(s) of specific header.
		 *	@param search: case-insensitive name of header
		 */
		std::vector<std::string> GetHeader(std::string const search)
		{
			std::vector<std::string>	ret;
			if (_header.count(search))
			{
				std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> _itr = _header.equal_range(search);
				for (_Container::iterator it = _itr.first; it != _itr.second; ++it)
					ret.push_back(it->second);
			}
			return ret;
		}
		
		/**
		 *	Remove header if exist; if not exist return true.
		 * 	Older iterator is invalid.
		 *	@param key: name of header
		 */
		bool remove(std::string key)
		{
			std::multimap<std::string, std::string>::iterator it;
			it = _header.find(toLowerCase(key));
			if (it != _header.end())
				_header.erase(toLowerCase(key));
			else
				return true;
			return false;
		}

		/**
		 * 	Add header to container; if exist nothing is change.
		 * 	Older iterator is invalid. If error occur return true.
		 *	@param header: complete header line
		 */
		bool add(std::string header)
		{
			std::string		key;
			_regex.Match(header, "^(^[^ ]+): (.*)(;.*?)");
			if (_regex.GetSize() < 3) return true;
			key = toLowerCase(_regex.GetMatch()[1].occurence);
			if (key.empty()) return true;
			key = toLowerCase(key);
			if (_header.count(key)) return true;
			for (size_t x = 2; x < _regex.GetSize(); x++)
				_header.insert(std::make_pair(key, _regex.GetMatch()[x].occurence));
			return false;
		}
		
		/**
		 * 	Set header to container; if exist delete old and replace with new.
		 * 	Older iterator is invalid. If error occur return true.
		 *	@param header: complete header line
		 */
		bool set(std::string header)
		{
			std::string		key;
			_regex.Match(header, "^(^[^ ]+): (.*)(;.*?)");
			if (_regex.GetSize() < 3) return true;
			key = toLowerCase(_regex.GetMatch()[1].occurence);
			if (key.empty()) return true;
			key = toLowerCase(key);
			if (!_header.count(key)) _header.erase(key);
			for (size_t x = 2; x < _regex.GetSize(); x++)
				_header.insert(std::make_pair(key, _regex.GetMatch()[x].occurence));
			return false;
		}		
};
#endif
