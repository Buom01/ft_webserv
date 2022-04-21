#ifndef __MIDDLEWARE_WRITE_BODY_HPP
# define __MIDDLEWARE_WRITE_BODY_HPP
# include "builtin.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "AEpoll.hpp"

class SendBodyFromFD: public AEpoll
{
	typedef	AEpoll	_parent;

	public:
		SendBodyFromFD(Log &);
		virtual ~SendBodyFromFD();
	public:
		bool	operator()(Request &, Response &);
};
bool	sendBodyFromBuffer(Request &, Response &);

#endif
