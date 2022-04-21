#ifndef __MIDDLEWARE_UPLOAD_HPP
# define __MIDDLEWARE_UPLOAD_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "AEpoll.hpp"

class Upload: public AEpoll
{
	typedef	AEpoll	_parent;
	protected:
		Log		&_logger;
	public:
		typedef struct	options_s
		{
			std::string	destination;
			std::string	public_root;
		}				options_t;
		options_t		options;
		Upload(Log &, std::string, std::string);
		virtual ~Upload();
	protected:
		bool	_eject_contentrange(Request &, Response &);
	public:
		bool	operator()(Request &, Response &);
};

#endif
