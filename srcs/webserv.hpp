#ifndef __WEBSERV_HPP
# define __WEBSERV_HPP

// default
# include "builtin.hpp"
# include "lib.hpp"

// includes
# include "Log.hpp"
# include "Epoll.hpp"
# include "Header.hpp"
# include "Url.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Parse.hpp"

// help
# include "Help.hpp"

// components
# include "components.hpp"

// middlewares
# include "AEpoll.hpp"
# include "IMiddleware.hpp"
# include "Cgi.hpp"
# include "body.hpp"
# include "eject.hpp"
# include "error.hpp"
# include "forbidden.hpp"
# include "graceful_shutdown.hpp"
# include "mimetypes.hpp"
# include "read.hpp"
# include "redirect.hpp"
# include "remover.hpp"
# include "upload.hpp"
# include "write_body.hpp"
# include "write_headers.hpp"

// serve
# include "Definition.hpp"
# include "Chain.hpp"
# include "Serve.hpp"

// static
# include "Static.hpp"

#endif
