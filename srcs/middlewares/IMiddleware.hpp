/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMiddleware.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 15:51:27 by badam             #+#    #+#             */
/*   Updated: 2022/01/07 16:35:12 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMIDDLEARE_HPP
# define IMIDDLEARE_HPP

# include "Serve.hpp"

class Request;
class Response;

class IMiddleware
{
    public:
        virtual void    operator()(Request &req, Response &res) = 0;
};

#endif