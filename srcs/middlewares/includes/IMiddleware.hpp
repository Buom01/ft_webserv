#ifndef __MIDDLEWAR_IMIDDLEWARE_HPP
# define __MIDDLEWAR_IMIDDLEWARE_HPP
# include "Request.hpp"
# include "Response.hpp"

class IMiddleware
{
    public:
        virtual bool    operator()(Request &req, Response &res) = 0;
};

#endif