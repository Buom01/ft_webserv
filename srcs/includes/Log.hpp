#ifndef __LOG_HPP
# define __LOG_HPP
# include "builtin.hpp"
# include "lib.hpp"

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

		void	log(std::string &method, int code, std::string &hostname, uint16_t port, std::string &path, std::string infos = "")
		{
			std::cout
				<< "[" << colorFromCode(code) << code << COLOR_RESET << "] " << method << " http://" << hostname;
			if (port != 80)
				std::cout << ":" << port;
			std::cout << path;
			if (infos.length())
				std::cout << " - " << infos;
			std::cout << std::endl;
		}

		void	warn(std::string warn_str, int error_nb= -1)
		{
			std::cerr
				<< COLOR_WARNING << "[WARNING] " << COLOR_RESET << warn_str;
			if (error_nb >= 0)
				std::cerr << ": " << strerror(error_nb);
			std::cerr << std::endl;
		}

		void	fail(std::string error_str, int error_nb = -1)
		{
			std::cerr
				<< COLOR_ERROR << "[ERROR] " << COLOR_RESET << error_str;
			if (error_nb >= 0)
				std::cerr << ": " << strerror(error_nb);
			std::cerr << std::endl;
		}
};

#endif
