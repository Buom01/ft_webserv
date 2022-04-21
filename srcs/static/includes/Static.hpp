#ifndef __STATIC_HPP
# define __STATIC_HPP
# include "builtin.hpp"
# include "components.hpp"
# include "Response.hpp"
# include "IMiddleware.hpp"

class Static: public IMiddleware
{
	typedef	IMiddleware	_parent;
	public:
		typedef struct	options_s
		{
			std::string					root;
			bool						directory_listing;
			std::vector<std::string>	indexes;
		}				options_t;
		options_t		options;
	public:
		Static(void);
		Static(options_t);
		virtual ~Static(void);
	protected:
		bool		serveDirectory(Response &, const std::string &, const std::string &);
		void		serveFile(Response &, const std::string &);
		std::string getIndex(const std::string &, const options_t &);
		void		redirect(Response &, std::string);
	public:
		bool	operator()(Request &, Response &);
};

#endif
