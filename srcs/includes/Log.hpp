/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 16:28:32 by badam             #+#    #+#             */
/*   Updated: 2022/04/16 01:21:27 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP
# include <cstring>
# include <string>
# include <iostream>
# include "http.hpp"
# define COLOR_RESET "\e[0m"
# define COLOR_ERROR "\e[91m"
# define COLOR_WARNING "\e[93m"
# define COLOR_SERVER_ERROR "\e[91m"
# define COLOR_CLIENT_ERROR "\e[91m"
# define COLOR_REDIRECT "\e[95m"
# define COLOR_SUCCESS "\e[92m"
# define COLOR_TITLE "\e[95m"
# define COLOR_ARGS "\e[96m"

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

		void	greeting(std::string address, uint16_t port)
		{
			std::cout << "Server is listening " << address
				<< " on port " << port << " !"
				<< std::endl;
		}

		void	stopping()
		{
			std::cout << "Server stopping..." << std::endl;
		}

		void	log(int code, std::string &hostname, uint16_t port, std::string &path, std::string infos = "")
		{
			std::cout
				<< "[" << colorFromCode(code) << code << COLOR_RESET << "] Generated http://" << hostname;
			if (port != 80)
				std::cout << ":" << port;
			std::cout << path;
			if (infos.length())
				std::cout << " - " << infos;
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

#endif
