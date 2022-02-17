/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/12 19:32:19 by cbertran          #+#    #+#             */
/*   Updated: 2022/02/17 02:59:26 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_HEADERS_CPP
# define __WRITE_HEADERS_CPP
# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"

static inline bool	writeBuffer(int fd, std::string &str, unsigned long &size, Log *logger)
{
	int	ret;

	if (size > str.length())
		size = str.length();

	ret = ::send(fd, str.c_str(), size, MSG_DONTWAIT | MSG_NOSIGNAL);

	if (ret == -1)
	{
		// @TODO: Es-ce que ça veut dire qu'on doit re-attendre sur epoll ?
		logger->warn("Failed to write buffer right now");
		return (false);
	}
	else
	{
		str.erase(0, ret);
		size -= ret;
		return (true);
	}

}

bool	addResponseHeaders(Request &, Response &res)
{
	Header				&h		= res.headers;
	std::stringstream	to_str;

	h.set("Server: FT_WebServ");
	to_str << res.body.str().length();
	h.set("Content-Length: " + to_str.str());
	if (res.code == C_OK && res.body.str().length() == 0)
		res.code = C_NO_CONTENT;
	
	return (true);
}

bool	serializeHeaders(Request &req, Response &res)
{
	if (req.closed())
		return (true);

	std::stringstream			startline;
	std::vector<std::string>	ListHeaders = res.headers.VectorOfEveryHeaders();
	
	startline << "HTTP/1.1 " << res.code << "\n";
	res.headers_buff.append(startline.str());

	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
	{
		res.headers_buff.append(*it);
		res.headers_buff.push_back('\n');
	}
	res.headers_buff.push_back('\n');

	return (true);
}

bool	sendHeader(Request &req, Response &res)
{
	unsigned long	bufferSize;

	if (req.closed())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	while (res.headers_buff.length())
	{
		bufferSize = 1024;
		
		while(bufferSize)
		{
			if (!writeBuffer(res.fd, res.headers_buff, bufferSize, res.logger))
				return (false);
		}
	}
	
	res.headers_sent = true;
	res.logger->log(res.code, req.pathname);  // should it be there or at the end ??
	
	return (true);
}

bool	sendBodyMockupFunction(Request &req, Response &res)
{
	if (req.closed())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	::send(res.fd, res.body.str().c_str(), res.body.str().length(), MSG_DONTWAIT | MSG_NOSIGNAL);
	::close(res.fd);

	res.sent = true;

	return (true);
}

/*
@Fait: Le sujet veux que l'envois se fasse aussi dans le même poll:
- "poll (ou équivalent) doit vérifier la lecture et l’écriture en même temps"
- "Il doit être non bloquant et n’utiliser qu’un seul poll(ou équivalent) pour toutes les E/S entre le client et le serveur (listen inclus)."


@TODO:
De plus on peut en générale savoir que le buffer du socket est saturé car send retounerais -1 avec errno à ERR_WOULDBLOCK, seulement:
- "La vérification de la valeur de errno est strictement interdite après une opération de lecture ou d’écriture."
Enfin, dans le cas où body peut être un fichier il y aura un autre epoll dédié à mettre en place

=> Solution:
- Si il retourne -1, se remttre dane l'epoll et attendre EPOLLOUT à nouveau (si ça marche ?? à priori c'est le but: https://stackoverflow.com/questions/3673828/proper-handling-of-ewouldblock-with-polling-on-a-non-blocking-socket)
- Bien écouter sur EPOLLHUP ("closed")
- Mettre un timeout
*/

#endif
