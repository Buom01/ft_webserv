/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMiddleware.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 15:51:27 by badam             #+#    #+#             */
/*   Updated: 2022/01/10 18:27:17 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMIDDLEARE_HPP
# define IMIDDLEARE_HPP

# include "Request.hpp"
# include "Response.hpp"

class Request;
class Response;

class IMiddleware
{
    public:
        virtual bool    operator()(Request &req, Response &res) = 0;
};

#endif