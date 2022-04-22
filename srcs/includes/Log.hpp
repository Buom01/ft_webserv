#ifndef __LOG_HPP
# define __LOG_HPP
# include "builtin.hpp"
# include "define.hpp"

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

		void	print_flux(int socket, const char *buffer, size_t length, const char *indicator)
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
			options.verbose = false;
		}

		virtual ~Log(void)
		{
			std::cout << "Server stopped." << std::endl;
		}

		struct	options_s
		{
			bool	verbose;
		}		options;

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
			if (!options.verbose)
				return ;
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

			if (length == 0)
				warn("Trying to send a buffer of length 0");
			if (options.verbose && ret > 0)
				print_flux(socket, buffer, ret, ">>>");
			return (ret);
		}

		void	logged_GNL(int socket, std::string line)
		{
			if (options.verbose)
				print_flux(socket, line.c_str(), line.size(), "<<<");
		}

		void	log_flux(int fd, char *buffer, ssize_t len, const char *prefix)
		{
			if (options.verbose)
				print_flux(fd, buffer, len, prefix);
		}
};

#endif
