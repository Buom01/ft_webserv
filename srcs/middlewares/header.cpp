#ifndef __PARSE_HEADER
# define __PARSE_HEADER
# include "../regex.hpp"
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>

/**
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
		Regex												_regex;
		std::map<std::string, std::vector<std::string> >	_header;
	
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
		std::map<std::string, std::vector<std::string> > GetEveryHeader()
		{ return _header; }
		
		/**
		 * 	Get value(s) of specific header.
		 *	@param search: case-insensitive name of header
		 */
		std::vector<std::string> GetHeader(std::string const search)
		{ return (_header[toLowerCase(search)]); }

		/**
		 * 	Return std::pair of header.
		 * 	If error occur, return empty pair.
		 *	@param line: complete header line
		 */
		std::pair<std::string,std::vector<std::string> > parse(std::string line)
		{
			std::string					key;
			std::vector<std::string>	value;

			_regex.Match(line, "^(^[^ ]+): (.*)(;.*?)");
			if (_regex.GetSize() < 3)
				return std::make_pair("", std::vector<std::string>());
			for (int x = 1; x < _regex.GetSize(); x++)
			{
				if (x == 1)
					key = toLowerCase(_regex.GetMatch()[x].occurence);
				else
					value.push_back(_regex.GetMatch()[x].occurence);
			}
			//_header.insert(std::make_pair(key, value));
			return std::make_pair(key, value);
		}

		/**
		 * 	Return std::string of well formated header.
		 * 	@param key: key of header
		 * 	@param value: value(s) of header
		 */
		std::string formate(std::string key, std::string value)
		{
			std::string temp = key;
			temp += ": ";
			temp += value;
			return temp;
		}

		/**
		 *	Remove header if exist; if not exist return true.
		 * 	Older iterator is invalid.
		 *	@param key: name of header
		 */
		bool remove(std::string key)
		{
			std::map<std::string, std::vector<std::string> >::iterator it;
			it = _header.find(toLowerCase(key));
			if (it != _header.end())
				_header.erase(it);
			else
				return true;
			return false;
		}

		/**
		 * 	Add header to container; if exist nothing is change.
		 * 	Older iterator is invalid if change. If error occur return true.
		 *	@param header: complete header line
		 */
		bool add(std::string header)
		{
			std::pair<std::string, std::vector<std::string> > pair = parse(header);
			if (pair.first.empty()) return true;
			std::map<std::string, std::vector<std::string> >::iterator it;
			it = _header.find(toLowerCase(pair.first));
			if (it != _header.end())
				return true;
			_header.insert(pair);
			return false;
		}
		
		/**
		 * 	Set header to container; if exist delete old and replace with new.
		 * 	Older iterator is invalid. If error occur return true.
		 *	@param header: complete header line
		 */
		bool set(std::string header)
		{
			std::pair<std::string, std::vector<std::string> > pair = parse(header);
			if (pair.first.empty()) return true;
			std::map<std::string, std::vector<std::string> >::iterator it;
			it = _header.find(toLowerCase(pair.first));
			if (it != _header.end())
				_header.erase(it);
			_header.insert(pair);
			return false;
		}		
};
#endif
