/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_body.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/03/14 18:38:26 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_BODY_CPP
# define __WRITE_BODY_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"

# include "utils/writeout.cpp"

// @TODO handle reponseFD
bool	sendBodyFromFD(Request &req, Response &res)
{
	if (res.sent)
		return (true);
	if (!res.response_fd)
		return (true);
	if (req.closed())
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	// ::send(res.fd, res.body.str().c_str(), res.body.length(), MSG_DONTWAIT | MSG_NOSIGNAL);

	res.sent = true;

	return (true);
}

bool	sendBodyFromBigBuffer(Request &req, Response &res)
{
	std::cout << "Call" << std::endl;
	ssize_t	send_ret = -1;

	if (res.sent)
		return (true);
	if (!req.await(EPOLLOUT))
		return (false);

	if (req.closed())  // Middleware has to not take care of it ? But they should have a cleanup
		return (true);

	while (send_ret != 0)
	{
		if (!res.sending_chunk_size)
		{
			if (!res.body.can_read())
				return (false);
			res.sending_chunk = res.body.get_next_chunk(&(res.sending_chunk_size));
			if (res.sending_chunk_size == 0)
				break ;
		}

		send_ret = send(res.fd, res.sending_chunk, res.sending_chunk_size, MSG_NOSIGNAL | MSG_DONTWAIT);
		if (send_ret == -1)
		{
			std::cout << "1" << std::endl;
			req.unfire(EPOLLOUT);
			return (false);
		}
		else
		{
			memmove(res.sending_chunk, res.sending_chunk + send_ret, res.sending_chunk_size - send_ret);
			res.sending_chunk_size -= send_ret;
			if (res.sending_chunk_size != 0)
			{
				std::cout << "2" << std::endl;
				std::cout << "sending_chunk_size is " << res.sending_chunk_size << std::endl;
				req.unfire(EPOLLOUT);
				return (false);
			}
		}
	}
	
	res.sent = true;

	return (true);
}

bool	finishBody(Request &, Response &res)
{
	::close(res.fd);
	std::cout << "Closed FD" << std::endl;

	return (true);
}

/*
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