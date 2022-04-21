#ifndef __SERVE_CHAIN_HPP
# define __SERVE_CHAIN_HPP
# include "builtin.hpp"
# include "Log.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Epoll.hpp"
# include "Definition.hpp"

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
