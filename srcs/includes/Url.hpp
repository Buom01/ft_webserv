/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Url.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 14:05:57 by cbertran          #+#    #+#             */
/*   Updated: 2022/03/28 13:57:51 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URL_HPP
# define __URL_HPP
# include <string>
# include <vector>
# include <utility>
# include <exception>
# include "Regex.hpp"

/**
 *	Create class for get specific part of url, every function return empty string if no occurence.
 *	Work like JS URL class, see https://developer.mozilla.org/en-US/docs/Web/API/URL for more exemple
 *	@param url: complete url
 */
class URL
{
	private:
		struct save_data
		{
			std::string	protocol;
			std::string	username;
			std::string	password;
			std::string	hostname;
			std::string	host;
			std::string	port;
			std::string	pathname;
			std::string	path;
			std::string	search;
			std::string	hash;
			std::vector<std::pair<std::string, std::string> > searchParams;
		}; save_data _data;
		std::string	_url;
		Regex		_regex;
		URL(void) {}
	private:
		void fillData()
		{
			_data.protocol = getProtocol();
			_data.username = getUsername();
			_data.password = getPassword();
			_data.hostname = getHostname();
			_data.host = getHost();
			_data.port = getPort();
			_data.pathname = getPathname();
			_data.path = getPath();
			_data.search = getSearch();
			if (!_data.search.empty() && _data.search != "q")
			{
				_data.search.erase(0,1);
				std::string del = ",";
				int start = 0;
				int end = _data.search.find(del);
				while (end != -1) {
					_regex.exec(_data.search.substr(start, end - start), "^(.*)=(.*)$", GLOBAL_FLAG);
					if (_regex.size() > 2)
						_data.searchParams.push_back(std::make_pair(_regex.match()[0].occurence, _regex.match()[1].occurence));
					start = end + del.size();
					end = _data.search.find(del, start);
				}
				_regex.exec(_data.search.substr(start, end - start), "^(.*)=(.*)$", GLOBAL_FLAG);
				if (_regex.size() > 2)
					_data.searchParams.push_back(std::make_pair(_regex.match()[0].occurence, _regex.match()[1].occurence));
			}
			_data.hash = getHash();
		}
	public:
		
		URL(std::string url) : _url(url)
		{ fillData(); }

		/**
		 * 	Update old url with new url
		 * 	@param url: complete new url
		 */
		void update(std::string url)
		{
			_url = url;
			fillData();
		}
	private:
		std::string getProtocol()
		{
			_regex.exec(_url, "^(http|https)", GLOBAL_FLAG);
			if (_regex.size() >= 2)
				return (_regex.match()[0].occurence);
			return "";
		}

		std::string getUsername()
		{
			_regex.exec(_url, "://(.*):(.*)@", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getPassword()
		{
			_regex.exec(_url, "://(.*):(.*)@", GLOBAL_FLAG);
			if (_regex.size() > 2)
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getHostname()
		{
			if (!password().empty())
				_regex.exec(_url, "@(\\w+\\.\\w+)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "://(\\w+\\.\\w+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}

		std::string getHost()
		{
			if (!password().empty())
				_regex.exec(_url, "@(\\w+\\.\\w+:?\\w+)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "://(\\w+\\.\\w+:?\\w+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}

		std::string getPort()
		{
			if (!password().empty())
				_regex.exec(_url, "@\\w+\\.\\w+:(\\w+)?", GLOBAL_FLAG);
			else
				_regex.exec(_url, "://\\w+\\.\\w+:(\\w+)?", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getPathname()
		{
			if (!password().empty())
				_regex.exec(_url, "@\\w+\\.\\w+:?\\w+([^ ?#]+)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "://\\w+\\.\\w+:?\\w+([^ ?#]+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "/";
		}
		
		std::string getPath()
		{
			if (!password().empty())
				_regex.exec(_url, "@\\w+\\.\\w+:?\\w+([^ #]+)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "://\\w+\\.\\w+:?\\w+([^ #]+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "/";
		}
		
		std::string getSearch()
		{
			_regex.exec(_url, "://.*(\\?[^ #]+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}

		std::string getHash()
		{
			_regex.exec(_url, "://.*(#[^ ]+)", GLOBAL_FLAG);
			if (_regex.size() > 1)
				return _regex.match()[0].occurence;
			return "";
		}

	public:
		std::string	href() { return _url; }
		std::string protocol() { return _data.protocol; }
		std::string username() { return _data.username; }
		std::string password() { return _data.password; }
		std::string hostname() { return _data.hostname; }
		std::string host() { return _data.host; }
		std::string port() { return _data.port; }
		std::string pathname() { return _data.pathname; }
		std::string path() { return _data.path; }
		std::string search() { return _data.search; }
		std::string hash() { return _data.hash; }
		
		/**
		 * 	Get value of specific element, return empty string if not found
		 * 	@param key: key of element
		 */
		std::string	searchParams(std::string key)
		{
			for (std::vector<std::pair<std::string, std::string> >::iterator it = _data.searchParams.begin(); it != _data.searchParams.end(); ++it)
				if (it->first == key) return it->second;
			return "";
		}

		/**
		 * 	Print data for debug
		 */
		void printData()
		{
			std::cout << "Href :" << href() << std::endl; 
			std::cout << "Protocol :" << protocol() << std::endl; 
			std::cout << "Username :" << username() << std::endl; 
			std::cout << "Password :" << password() << std::endl; 
			std::cout << "Host :" << host() << std::endl; 
			std::cout << "Hostname :" << hostname() << std::endl; 
			std::cout << "Port :" << port() << std::endl;
			std::cout << "Path :" << path() << std::endl;
			std::cout << "Pathname :" << pathname() << std::endl; 
			std::cout << "Search :" << search() << std::endl;
			std::cout << "Hash :" << hash() << std::endl;
			std::cout << "searchParams:";
			for (std::vector<std::pair<std::string, std::string> >::iterator it = _data.searchParams.begin(); it != _data.searchParams.end(); ++it)
				std::cout << it->first << "=" << it->second << "|";
			std::cout << std::endl;
		}
};

#endif
