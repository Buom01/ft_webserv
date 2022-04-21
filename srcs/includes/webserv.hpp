#ifndef __WEBSERV_HPP
# define __WEBSERV_HPP

// c lib
# include <arpa/inet.h>
# include <bits/stdc++.h>
# include <regex.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

// c++ lib
# include <algorithm>
# include <cctype>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <fstream>
# include <iterator>
# include <iostream>
# include <map>
# include <ostream>
# include <set>
# include <stdexcept>
# include <sstream>
# include <string>
# include <utility>
# include <vector>

// lib
# include "lib.hpp"

// includes
# include "Log.hpp"
# include "Epoll.hpp"
# include "Header.hpp"
# include "Url.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Parse.hpp"
# include "components/page.hpp"
# include "components/utils.hpp"
# include "components/list.hpp"

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

#endif
