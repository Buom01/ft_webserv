#ifndef __URL_HPP
# define __URL_HPP
# include "builtin.hpp"
# include "lib.hpp"

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
			//"^(http|https)://(.+:.+@)?([^:/]+):?([0-9]+)?([^?#]*)(\\?[^#]+)?(#.+)?"
			_data.protocol = getProtocol();
			_data.username = getUsername();
			_data.password = getPassword();
			_data.host = getHost();
			_data.hostname = getHostname();
			_data.port = getPort();
			_data.path = getPath();
			_data.pathname = getPathname();
			_data.search = getSearch();
			_data.hash = getHash();
			if (!_data.search.empty())
			{
				std::vector<std::string> _split = split(_data.search, "&");
				for (std::vector<std::string>::const_iterator it = _split.begin(); it != _split.end(); it++)
				{
					std::vector<std::string> _keys = split(*it, "=");
					_data.searchParams.push_back(std::make_pair(_keys[0], _keys[1]));
				}
			}
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
			if (_regex.size())
				return (_regex.match()[0].occurence);
			return "";
		}

		std::string getUsername()
		{
			_regex.exec(_url, "^.*://(.*):.*@", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getPassword()
		{
			_regex.exec(_url, "^.*://.*:(.*)@", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getHost()
		{
			if (!password().empty())
				_regex.exec(_url, "^.*://.*:.*@([^/]+)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "^.*://([^/]+)", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "";
		}
		
		std::string getHostname()
		{
			std::vector<std::string> _split = split(host(), ":");
			if (_split.size())
				return _split[0];
			return "";
		}

		std::string getPort()
		{
			std::vector<std::string> _split = split(host(), ":");
			if (_split.size() == 2)
				return _split[1];
			return "";
		}
		
		std::string getPath()
		{
			_regex.exec(_url, "^.*://[^/]+(.*)$", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "/";
		}
		
		std::string getPathname()
		{
			if (!password().empty())
				_regex.exec(_url, "^.*://.*:.*@[^/]+([^?#]*)", GLOBAL_FLAG);
			else
				_regex.exec(_url, "^.*://[^/]+([^?#]*)", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "/";
		}
		
		std::string getSearch()
		{
			_regex.exec(_url, "://.*\\?([^ #]+)", GLOBAL_FLAG);
			if (_regex.size())
				return _regex.match()[0].occurence;
			return "";
		}

		std::string getHash()
		{
			_regex.exec(_url, "://.*#([^ ]+)", GLOBAL_FLAG);
			if (_regex.size())
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
