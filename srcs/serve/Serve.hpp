/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serve.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 23:42:44 by badam             #+#    #+#             */
/*   Updated: 2021/07/10 19:38:58 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVE_HPP
# define SERVE_HPP

# include <exception>
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <unistd.h>
# include <vector>

class Request;
class Response;

typedef	struct sockaddr		sockaddr_t;
typedef	struct sockaddr_in	sockaddr_in_t;

typedef struct server_address_s
{
	std::string		address;
	int				port;
	sockaddr_in_t	sockaddr_in;
	sockaddr_t		*sockaddr;
	socklen_t		len;
} server_address_t;

typedef	std::vector<server_address_t>	addresses_t;

typedef	void (*middleware_t)(Request&, Response&);

typedef enum flag_e
{
	F_NORMAL,
	F_ERROR,
	F_BOTH
} flag_t;

# include "Serve.cpp"

#endif
