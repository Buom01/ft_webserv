/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbertran <cbertran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 22:02:44 by badam             #+#    #+#             */
/*   Updated: 2022/03/31 14:09:27 by cbertran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Header.hpp"

int main(int argc, char **argv)
{
	Header						h;
	std::vector<std::string>	ListHeaders;
	Header::_headers			MapHeaders;
	
	std::cout << "Setting input headers:" << std::endl;
	for (int i = 1; i < argc; i++)
	{
		std::cout << "- \"" << argv[i] << "\": ";
		if (h.set(argv[i]))
			std::cout << "KO";
		else
			std::cout << "OK";
		std::cout << std::endl;
	}

	std::cout << "Parsing headers" << std::endl;
	MapHeaders = h.map();
	for (Header::_headers_it it = MapHeaders.begin(); it != MapHeaders.end(); it++)
	{
		std::cout << (*it).first << "=";
		for (Header::_vectors::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
			std::cout << (*it2) << "|";
		std::cout << std::endl;
	}


	std::cout << "Serializing headers:" << std::endl;
	ListHeaders = h.headers();
	for (std::vector<std::string>::iterator it = ListHeaders.begin(); it != ListHeaders.end(); ++it)
	{
		std::cout << "- \"" << *it << "\"" << std::endl;
	}
	std::cout << std::endl;
}