/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 14:41:25 by cbertran          #+#    #+#             */
/*   Updated: 2021/07/31 01:21:35 by badam            ###   ########.fr       */
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
		/**
		 * 	Return parsed header containing key followed by value(s)
		 *  or an empty vector if match failed.
		 */
		std::vector<std::string>	parseHeader(std::string raw)
		{
			match_t						*match;
			std::string					key;
			std::string					rawValue;
			std::vector<std::string>	results;

			if (!(match = _regex.Match(raw, "^([^ ]+): ([^,].*)$"))) return results;
			key = toLowerCase(match[1].occurence);
			rawValue = match[2].occurence;

			results.push_back(key);
			_regex.Init(" *([^,]+),? *", rawValue);
			while ((match = _regex.Exec()))
				results.push_back(match[1].occurence);
			
			return (results);
		}

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
		 * 	Get vector of every HTTP header in map
		 */
		std::vector<std::string> VectorOfEveryHeaders()
		{
			std::vector<std::string>	keys;
			std::vector<std::string>	headers;
			for (_Container::iterator it = _header.begin(); it != _header.end(); ++it)
			{
				if (std::find(keys.begin(), keys.end(), (*it).first) == keys.end())
					keys.push_back((*it).first);
			}
			for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); ++it)
				headers.push_back(HTTPheader(*it));
			return (headers);
		}

		/**
		 * 	Return well formated string represent HTTP header
		 * 	@param it: iterator of multimap
		 */
		std::string HTTPheader(_Container::iterator &it)
		{
			return (HTTPheader((*it).first));
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
			{
				if (it2 != HTTP.begin())
					ret += ",";
				ret += *it2;
			}
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
			std::vector<std::string>	parsedHeader	= parseHeader(header);
			std::string					key;
			
			if (parsedHeader.size() < 2) return true;
			key = parsedHeader[0];
			if (_header.count(key)) return true;
			for (size_t i = 1; i < parsedHeader.size(); ++i)
				_header.insert(std::make_pair(key, parsedHeader[i]));
			return false;
		}
		
		/**
		 * 	Set header to container; if exist delete old and replace with new.
		 * 	Older iterator is invalid. If error occur return true.
		 *	@param header: complete header line
		 */
		bool set(std::string header)
		{
			std::vector<std::string>	parsedHeader	= parseHeader(header);
			std::string					key;
			
			if (parsedHeader.size() < 2) return true;
			key = parsedHeader[0];
			if (_header.count(key)) _header.erase(key);
			for (size_t i = 1; i < parsedHeader.size(); ++i)
				_header.insert(std::make_pair(key, parsedHeader[i]));
			return false;
		}		
};
#endif
