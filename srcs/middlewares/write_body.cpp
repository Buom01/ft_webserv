/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_body.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 21:55:09 by badam             #+#    #+#             */
/*   Updated: 2022/02/17 22:02:03 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WRITE_BODY_CPP
# define __WRITE_BODY_CPP

# include "Request.hpp"
# include "Response.hpp"
# include "Serve.hpp"

# include "utils/writeout.cpp"


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