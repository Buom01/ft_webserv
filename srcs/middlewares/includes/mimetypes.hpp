#ifndef __MIDDLEWARE_MIMETYPES_HPP
# define __MIDDLEWARE_MIMETYPES_HPP
# include "webserv.hpp"

class Mimetypes: public IMiddleware
{
	typedef	IMiddleware							_parent;
	typedef	std::map<std::string, std::string>	mimetypes_t;
	public:
		typedef struct	options_s
		{
			std::string	fallback;
			std::string	directory_default;
			mimetypes_t	mimetypes;
		}				options_t;
		options_t		options;
	public:
		Mimetypes();
		Mimetypes(options_t);
		virtual ~Mimetypes();
		void	useDefaults();
		void	add(std::string, std::string);
		std::string	getMimetype(std::string &);
	public:
		bool	operator()(Request &, Response &);
};

#endif
