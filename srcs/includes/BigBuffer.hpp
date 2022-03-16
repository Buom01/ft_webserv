/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BigBuffer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:24:53 by badam             #+#    #+#             */
/*   Updated: 2022/03/16 00:33:44 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIGBUFFER_HPP
# define BIGBUFFER_HPP

# include "Epoll.hpp"
# include "File.hpp"

/*
** This class aim to provide a easy to use interface
** to write block of buffer, in RAM of DISK according
** to its size, using epoll.
** Its main adventage is that is will never accept to
** append partial buffer. For instance if the whole
** block can't be wrote on the disk, it will store it
** in inner buffer, and conciliate both.
** Constraints:
**   - Each block can't overpass buffer maxsize.
*/
class	BigBuffer
{
	protected:
		Log			&_logger;
		
		uint32_t	_events;
		Epoll		_epoll;
		bool		_use_file;
		size_t		_buffer_maxsize;
		
		char		*_buffer;
		size_t		_buffer_size;
		int			_tmp_fd;
		size_t		_tmp_size;

		void	_create_file()
		{
			char	tmp_templatename[]	= "bigbuffer_XXXXXX";
			
			_tmp_fd = open("bigbuffer_XXXXXX", O_NOATIME | O_NOFOLLOW | O_NONBLOCK | O_DIRECT, O_RDWR);
			// _tmp_fd = mkostemp(tmp_templatename, O_NOATIME | O_NOFOLLOW | O_NONBLOCK);
			if (_tmp_fd > 0)
			{
				_use_file = true;
				_epoll.add(_tmp_fd, ET_BIGBUFFER, NULL);
			}
		}

		void	_destroy_file()
		{
			if (_use_file)
			{
				_epoll.remove(_tmp_fd);
				close(_tmp_fd);
				_use_file = false;
				_tmp_fd = -1;
			}
		}

		bool	_flush()
		{
			if (_use_file && _buffer_size)
			{
				if (!(_events & EPOLLIN))
					return (false);

				lseek(_tmp_fd, 0, SEEK_END);

				ssize_t	wrote = write(_tmp_fd, _buffer, _buffer_size);

				if (wrote == -1)
				{
					_events &= ~EPOLLIN;
					return (false);
				}


				_tmp_size += wrote;
				_buffer_size -= wrote;
				
				if (_buffer_size)
				{
					_events &= ~EPOLLIN;
					return (false);
				}
			}
			
			return (true);
		}

		void	_refresh_events()
		{
			events_t				events			= _epoll.accept();
			events_t::iterator		it				= events.begin();

			while (it != events.end())
			{
				std::cout << "Got: " << static_cast<event_data_t *>(it->data.ptr)->fd << std::endl;
				if (static_cast<event_data_t *>(it->data.ptr)->fd == _tmp_fd)
					_events |= it->events;
				else
					_logger.warn("Event happened on unregistred FD");
				++it;
			}
		}

	public:
		size_t		next_chunk;

		BigBuffer(Log &logger, size_t buffer_maxsize = 1024):
			_logger(logger),
			_events(0),
			_epoll(logger),
			_use_file(false),
			_buffer_maxsize(buffer_maxsize),
			_buffer(NULL),
			_buffer_size(0),
			_tmp_fd(-1),
			_tmp_size(0),
			next_chunk(0)
		{  // @TODO :verify that throw are handled, if alloc failed
			_buffer = new char[buffer_maxsize];
		}
		
		virtual ~BigBuffer()
		{
			std::cout << "BigBuff destoying" << std::endl;
			// _destroy_file();
			// delete []_buffer;
		}

		size_t	chunk_size()
		{
			return (_buffer_maxsize);
		}

		size_t	length()
		{
			return (_buffer_size + _tmp_size);
		}

		bool	can_write()
		{
			if (!_use_file)
				return (true);
			else
			{
				_refresh_events();
				return (_events & EPOLLIN && _flush());
			}
		}

		bool	can_read()
		{
			if (!_use_file)
				return (_flush());
			else
			{
				_refresh_events();
				return (_events & EPOLLOUT && _flush());
			}
		}

		bool	append(char *buff, size_t len)
		{
			if (!can_write())
				return (false);
			if (len > _buffer_maxsize)
			{
				_logger.warn("Writting more that buffer maxsize is forbidden");
				len = _buffer_maxsize;
			}

			if (_use_file)
			{
				if (!_flush())
					return (false);
				memcpy(_buffer, buff, len);
				_buffer_size += len;
				_flush();

				return (true);
			}
			else
			{
				if (len + _buffer_size > _buffer_maxsize)
				{
					_create_file();
					return (false);
				}
				else
				{
					memcpy(_buffer + _buffer_size, buff, len);
					_buffer_size += len;

					if (_buffer_size == _buffer_maxsize)
						_create_file();
					return (true);
				}
			}
		}

		/*
		**  Will always fullfil buffer with chat[chunk_size()] if possible
		**  We force to flush through read to either use file or RAM
		*/
		char	*get_chunk(size_t *buff_size, size_t chunk_number)
		{
			ssize_t	read_ret	= 0;

			*buff_size = 0;
			if (_use_file)
			{
				if (!can_read())
					return (NULL);

				lseek(_tmp_fd, chunk_number * chunk_size(), SEEK_SET);

				std::cout << "Reading at " << chunk_number * chunk_size() << std::endl;
				
				read_ret = read(_tmp_fd, _buffer, chunk_size());
				if (read_ret == -1)
				{
					_events &= ~EPOLLOUT;
					return (NULL);
				}
				*buff_size = read_ret;
			}
			else
			{
				if (chunk_number)
					*buff_size = 0;
				else
					*buff_size = _buffer_size;
			}
			return (_buffer);
		}


		char	*get_next_chunk(size_t *buff_size)
		{
			return (get_chunk(buff_size, next_chunk++));
		}

		/*
		** May need @TODO: operator[] and insert at for random access as upload
		*/
};

#endif