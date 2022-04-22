#ifndef __SERVE_SERVE_HPP
# define __SERVE_SERVE_HPP
# include "builtin.hpp"
# include "Log.hpp"
# include "Epoll.hpp"
# include "IMiddleware.hpp"
# include "Definition.hpp"
# include "Chain.hpp"

class	Serve
{
	Epoll			_epoll;
	binds_t			_binds;
	Chain			_response_chain;
	bool			_alive;

	public:
		Log				&logger;
		Serve(Log &);
		virtual ~Serve();
	private:
		void			_destroyBind(server_bind_t *);
		std::string		_netIpToStr(in_addr_t);
		in_addr_t		_ipFromHost(std::string);
		server_bind_t	*_hasBind(std::string &, uint16_t &);
		server_bind_t	*_bindForFD(int);
	public:
		server_bind_t	*bind(std::string, uint16_t, std::vector<std::string> &);
		void			begin();
		void			use(
			IMiddleware &,
			chain_flag_t = F_NORMAL,
			method_t = M_ALL,
			std::string = "",
			ServerConfig = ServerConfig()
		);
		void			use(
			bool (&middleware)(Request&, Response&),
			chain_flag_t = F_NORMAL,
			method_t = M_ALL,
			std::string = "",
			ServerConfig = ServerConfig()
		);
		RunningChain	*exec(
			int,
			server_bind_t *,
			std::string,
			uint32_t
		);
		bool			retake(RunningChain *, uint32_t);
		void			retake();
		void			accept();
		bool			alive();
		void			stop();

		class	ServerException: public std::runtime_error
		{
			public:
				ServerException(const std::string & = "Unknown internal server error.");
		};

		class	ServerSocketException: public ServerException
		{
			public:
				ServerSocketException(const std::string & = "Server socket exception.");
		};
};

#endif
