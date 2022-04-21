#ifndef __SERVE_CHAIN_HPP
# define __SERVE_CHAIN_HPP
# include "webserv.hpp"

class	Serve
{
	Epoll			_epoll;
	binds_t			_binds;
	Chain			_response_chain;
	bool			_alive;

	public:
		Log				logger;
		Serve();
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
		void			use(IMiddleware &, chain_flag_t, method_t, std::string, ServerConfig);
		void			use(bool (&middleware)(Request&, Response&), chain_flag_t, method_t, std::string, ServerConfig);
		RunningChain	*exec(int, server_bind_t *, std::string, uint32_t);
		bool			retake(RunningChain *, uint32_t);
		void			retake();
		void			accept();
		bool			alive();
		void			stop();

		class	ServerException: public std::runtime_error
		{
			public:
				ServerException(const std::string &);
		};

		class	ServerSocketException: public ServerException
		{
			public:
				ServerSocketException(const std::string &);
		};
};

#endif
