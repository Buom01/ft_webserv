/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 14:49:27 by badam             #+#    #+#             */
/*   Updated: 2021/07/11 15:11:18 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "regex.hpp"

int	main(int argc, char **argv)
{
	Regex	regex;
	size_t	i		= 0;

	if (argc < 3)
	{
		std::cout << "Usage: [TEXT] [REGEX]" << std::endl;
		return (0);
	}
	argv++;
	std::cout << "Testing: " << argv[0] << std::endl;
	std::cout << "With: " << argv[1] << std::endl;
	regex.Match(argv[0], argv[1]);

	std::cout << "Found " << regex.GetSize() << " matches." << std::endl;
	std::cout << "[" << std::endl;
	while (i < regex.GetSize())
	{
		std::cout << regex.GetMatch()[i].occurence;

		if (++i < regex.GetSize())
			std::cout << "," << std::endl;
		else
			std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}
