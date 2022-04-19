/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 12:27:56 by badam             #+#    #+#             */
/*   Updated: 2022/04/20 01:26:19 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <time.h>
# include "Log.hpp"
# include "Header.hpp"
# include "http.hpp"
# include "utils.hpp"

typedef enum	chain_state_e
{
	CS_OTHER = 0,
	CS_AWAIT_EVENT
}				chain_state_t;

class Request
{
	public:
		struct timespec		start;
		server_bind_t		*interface;
		int					fd;
		std::string			client_ip;
		Log					&logger;
		uint32_t			events;
		bool				&alive;
		chain_state_t		*state;
		struct timespec		*wait_since;
		bool				*wait_timeout;
		std::string			buff;
		
		std::string			method_str;
		method_t			method;
		std::string			raw_pathname;
		std::string			protocol;
		std::string			host;
		std::string			hostname;
		std::string			port;
		std::string			pathname;
		std::string			username;
		std::string			password;
		std::string			querystring;
		std::string			trusted_complete_pathname;
		std::string			trusted_pathname;
		std::string			http_version;
		Header				headers;
		size_t				upload_chunksize;
		size_t				upload_remainingsize;
		int					upload_fd;
		std::string			upload_fd_buff;
		std::string			upload_filename;
		std::string			upload_filename_tmp;

		bool				body_header_parsed;
		bool				body_read_is_finished;
		bool 				body_chuncked;

		ssize_t				body_chunk_size;
		ssize_t				body_read_size;
		ssize_t				body_is_chunk;

		ssize_t				body_length;

		std::string 		body_boundary;
		std::string 		body_boundary_end;
		std::string 		body;

		Request(int connection, server_bind_t *_interface, std::string &_client_ip, uint32_t _events, bool &_alive, Log &_logger) :
			start(get_time()),
			interface(_interface),
			fd(connection),
			client_ip(_client_ip),
			logger(_logger),
			events(_events),
			alive(_alive),
			state(NULL),
			wait_since(NULL),
			wait_timeout(NULL),
			buff(""),
			
			method_str(""),
			method(M_UNKNOWN),
			raw_pathname(""),
			protocol(""),
			host(""),
			hostname(""),
			port(""),
			pathname(""),
			username(""),
			password(""),
			querystring(""),
			trusted_complete_pathname("/"),
			trusted_pathname("/"),
			http_version(""),
			headers(),
			upload_chunksize(10240),
			upload_remainingsize(0),
			upload_fd(0),
			upload_fd_buff(""),
			upload_filename(""),
			upload_filename_tmp(""),

			body_header_parsed(false),
			body_read_is_finished(false),
			body_chuncked(false),

			body_chunk_size(0),
			body_read_size(0),
			body_is_chunk(false),

			body_length(0),
			body_boundary(""),
			body_boundary_end(""),
			body("")
		{
			buff.reserve(SERVER_BUFFER_SIZE);
		}

		virtual ~Request()
		{}

		bool	await(uint32_t _events)
		{
			if (events & _events)
			{
				*state = CS_OTHER;
				return (true);
			}
			else
			{
				*state		= CS_AWAIT_EVENT;
				*wait_since	= get_time();
				return (false);
			}
		}

		bool	fire(uint32_t _events)
		{
			if (!(~events & _events))
			{
				return (false);
			}
			else
			{
				events = events | _events;
				return (true);
			}
		}

		void	unfire(uint32_t _events)
		{
			events = events &~ _events;
		}

		bool	timeout()
		{
			return (
				get_elasped_ns(start) >= (int64_t)30 * 1000000000
				|| *wait_timeout
			);
		}

		bool	closed()
		{
			return (events & EPOLLHUP);
		}

		bool	finish()
		{
			return (closed() || (!alive && timeout()));
		}
};

#endif
