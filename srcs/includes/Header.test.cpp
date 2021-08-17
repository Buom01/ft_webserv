/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 22:02:44 by badam             #+#    #+#             */
/*   Updated: 2021/07/29 22:21:43 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Header.hpp"

int main(int argc, char **argv)
{
    Header                              h;
    int                                 i           = 1;
    std::vector<std::string>            ListHeaders;
    std::vector<std::string>::iterator  it;
	
    std::cout << "Setting input headers:" << std::endl;
    while (i < argc)
    {
        std::cout << "- \"" << argv[i] << "\": ";
        if (h.set(argv[i]))
            std::cout << "KO";
        else
            std::cout << "OK";
        std::cout << std::endl;
        ++i;
    }
    std::cout << std::endl;

    std::cout << "Serializing headers:" << std::endl;
    ListHeaders = h.VectorOfEveryHeaders();
    for (it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
    {
        std::cout << "- \"" << *it << "\"" << std::endl;
    }
    std::cout << std::endl;
}