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

		void	print_flux(int socket, const char *buffer, size_t length, std::string indicator)
		{
			std::stringstream	prefix;
			std::stringstream	print;
			size_t				i		= 0;
			size_t				j;

			prefix << COLOR_DEBUG << socket << " " << indicator << " " << COLOR_RESET;

			print << (prefix.str());

			while (i < length)
			{
				switch (buffer[i])
				{
					case '\n':
						if (i + 1 < length)
						{
							print << ("\\n\n");
							print << (prefix.str());
						}
						else
							print << ("\\n");
						break;
					case '\r':
						print << ("\\r");
						break;
					default:
						j = i;
						while (j < length && buffer[j] != '\n' && buffer[j] != '\r')
							++j;
						if (j - i > 255)
							print << COLOR_DEBUG << "[" << j - i << " characters]" << COLOR_RESET;
						else
							print.write(buffer + i, j - i);
						i = j - 1;
				}
				++i;
			}

			std::cout << print.str() << std::endl;
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

		void	info(std::string info_str)
		{
			std::cout
				<< COLOR_INFO << "[INFO] " << COLOR_RESET
				<< info_str << std::endl;
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

		ssize_t	logged_send(int socket, const char *buffer, size_t length, int flags)
		{
			ssize_t	ret = send(socket, buffer, length, flags);

			if (ret > 0)
				print_flux(socket, buffer, ret, ">>>");
			if (length == 0)
				warn("Trying to send a buffer of length 0");
			return (ret);
		}

		void	log_read(int fd, char *buffer, ssize_t ret)
		{
			print_flux(fd, buffer, ret, "READ");
		}

		void	logged_GNL(int socket, std::string line)
		{
			print_flux(socket, line.c_str(), line.size(), "<<<");
		}
};

#endif
