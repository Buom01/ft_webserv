/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Regex2.test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 14:49:27 by badam             #+#    #+#             */
/*   Updated: 2021/07/31 00:34:02 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Regex.hpp"

int	main(int argc, char **argv)
{
	size_t	i;
	match_t	*match;

	if (argc < 3)
	{
		std::cout << "Usage: [TEXT] [REGEX]" << std::endl;
		return (0);
	}
	argv++;
	std::cout << "Testing: \"" << argv[0] << "\"" << std::endl;
	std::cout << "With: \"" << argv[1] << "\"" << std::endl;

	Regex	regex(argv[1], argv[0]);

	std::cout << "Regex would return " << regex.GetSize() - 1 << " groups." << std::endl;
	while ((match = regex.Exec()))
	{
		std::cout << "[" << std::endl;
		i = 0;
		while (i < regex.GetSize() && match[i])
		{
			std::cout << "\t\"" << match[i].occurence << "\"";

			if (++i < regex.GetSize())
				std::cout << "," << std::endl;
			else
				std::cout << std::endl;
		}
		std::cout << "]" << std::endl;
	}
}
