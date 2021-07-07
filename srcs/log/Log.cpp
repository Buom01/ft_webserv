/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 15:50:29 by badam             #+#    #+#             */
/*   Updated: 2021/07/07 13:31:14 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <string>
#include <iostream>
#include "Log.hpp"


class	Log
{
	private:
		std::string	colorFromCode(int code)
		{
			if (code >= 500)
				return (COLOR_SERVER_ERROR);
			else if (code >= 400)
				return (COLOR_CLIENT_ERROR);
			else if (code >= 300)
				return (COLOR_REDIRECT);
			else
				return (COLOR_SUCCESS);
		}

	public:
		Log(void)
		{
			std::cout << COLOR_RESET << "Starting server..." << std::endl;
		}

		~Log(void)
		{
			std::cout << "Server stopped." << std::endl;
		}

		void	greeting(std::string address, int port)
		{
			std::cout << "Server is listening " << address
				<< " on port " << port << " !"
				<< std::endl;
		}

		void	log(int code, std::string path, std::string infos = "")
		{
			std::cout
				<< "[" << colorFromCode(code) << code << COLOR_RESET << "] "
				<< "Got \"" << path << "\"";
			if (infos.length())
				std::cout << ": " << infos;
			std::cout << std::endl;
		}

		void	warn(std::string warn_str, int error_nb= -1)
		{
			std::cout
				<< COLOR_WARNING << "[WARNING] " << COLOR_RESET << warn_str;
			if (error_nb >= 0)
				std::cout << ": " << strerror(error_nb);
			std::cout << std::endl;
		}

		void	fail(std::string error_str, int error_nb = -1)
		{
			std::cout
				<< COLOR_ERROR << "[ERROR] " << COLOR_RESET << error_str;
			if (error_nb >= 0)
				std::cout << ": " << strerror(error_nb);
			std::cout << std::endl;
		}
};
