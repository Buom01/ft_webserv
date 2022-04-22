#ifndef __LIB_UTILS_HPP
# define __LIB_UTILS_HPP
# include "http.hpp"
# include "Parse.hpp"

struct timespec	get_time();
method_t		method(Parse::s_allow);
std::string		*replace_all(std::string*, const std::string&, const std::string&);
std::string		bind_to_string(server_bind_t *);
size_t			min(size_t, size_t);
int64_t			difftimespec_ns(const struct timespec, const struct timespec);
int64_t			get_elasped_ns(struct timespec);
float			difftimespec_ms(const struct timespec, const struct timespec);
float			get_elasped_ms(struct timespec);
int64_t			get_difftime_ns(struct timespec, struct timespec);
float			get_difftime_ms(struct timespec, struct timespec);

#endif
