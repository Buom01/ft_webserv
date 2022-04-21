#ifndef __SERVE_CHAIN_HPP
# define __SERVE_CHAIN_HPP
# include "webserv.hpp"

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
			int,
			server_bind_t *,
			std::string &,
			uint32_t,
			bool &,
			Log &,
			chain_t::iterator
		);
		virtual ~RunningChain();
		bool	poll_timeout();
};

typedef std::vector<RunningChain *>    running_chains_t;

class   Chain
{
    protected:
		Epoll				&_epoll;
        chain_t				_raw_chain;
		running_chains_t	_running;
		bool				_alive;
	protected:
		void	_log_error(RunningChain &, const std::exception &);
		bool	_hasInterface(server_bind_t *, interfaces_t &);
		bool	_hasHostname(std::string &, hostnames_t &, hostnames_t &);
		bool	_isSubpathOf(std::string &, std::string &);
		bool	_canUseLink(chain_link_t &, Request &, Response &);
		void	_root_at_locationblock(chain_link_t &, Request &);
		bool	_run(RunningChain &);
		void	_handle_exception(RunningChain &, const std::exception &);
		bool	_exec_instance(RunningChain &);
    public:
        Chain(Epoll &epoll);
        ~Chain();
	public:
		void unsafe_remove_instance(RunningChain *);
		void	use(
			IMiddleware &,
			chain_flag_t,
			method_t,
			std::string &,
			ServerConfig &
		);
		void	use(bool (&middleware)(Request&, Response&), chain_flag_t, method_t, std::string &, ServerConfig &);
        RunningChain	*exec(int, server_bind_t *, std::string &, uint32_t, Log &);
		bool	retake(RunningChain *, uint32_t);
		void	retake();
		bool	alive();
		void	stop();
};

#endif
