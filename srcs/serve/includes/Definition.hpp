#ifndef __SERVE_DEFINITION_HPP
# define __SERVE_DEFINITION_HPP
# include "builtin.hpp"
# include "lib.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "IMiddleware.hpp"

typedef	std::set<std::string>			hostnames_t;
typedef	std::vector<server_bind_t *>	interfaces_t;

class	ServerConfig
{
	public:
		hostnames_t		hostnames;
		interfaces_t	interfaces;
		ServerConfig();
};

typedef struct	middleware_s
{
	IMiddleware	*obj;
	bool		(*fct)(Request&, Response&);
}				middleware_t;

typedef struct chain_link_s
{
	chain_flag_t	flag;
	method_t		methods;
	std::string		pathname;
	ServerConfig	serverConfig;
	middleware_t	middleware;
} chain_link_t;

typedef std::vector<chain_link_t>	    chain_t;

class	RunningChain
{
	public:
		Request 			req;
		Response			res;
		uint32_t			events;
		chain_t::iterator	pos;
		chain_state_t		state;
		struct timespec		wait_since;
		bool				wait_timeout;
	public:
		RunningChain(
			int connection,
			server_bind_t *interface,
			std::string &client_ip,
			uint32_t _events,
			bool &_alive,
			Log &logger,
			chain_t::iterator _pos
		);
		virtual ~RunningChain();
		bool	poll_timeout();
};

typedef std::vector<RunningChain *>    running_chains_t;

#endif
