#ifndef __LIB_GNL_HPP
# define __LIB_GNL_HPP
# include <string>
# include <sys/types.h>
# include "http.hpp"
# include "Log.hpp"

size_t	get_line_end(std::string &);
bool	get_line(std::string &, std::string *, std::string &, bool);
bool	get_next_line_string(int, std::string &, std::string &, Log &);

#endif
