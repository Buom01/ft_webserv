/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GNL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <badam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 17:06:14 by badam             #+#    #+#             */
/*   Updated: 2022/04/13 00:39:57 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __GNL_HPP
# define __GNL_HPP
# include <string>
# include <stdlib.h>
# include <unistd.h>

static int		buff_strip_used(ssize_t len, char *buffer, ssize_t buffer_size)
{
	int		same_line	= 1;

	if (len + 1 < buffer_size && (buffer[len] == '\n' || buffer[len] == '\r'))
	{
		++len;
		if ((buffer[len] == '\n' || buffer[len] == '\r') && buffer[len - 1] != buffer[len])
			++len;
		same_line = 0;
	}
	if (len)
	{
		memmove(buffer, buffer + len, buffer_size - len);
		buffer[buffer_size - len] = '\0';
	}
	return (same_line);
}

static ssize_t	buff_curnt_line_len(char *buffer, ssize_t buffer_size)
{
	ssize_t	len;

	len = 0;
	while (len < buffer_size && *buffer && *buffer != '\r' && *buffer != '\n')
	{
		len++;
		buffer++;
	}
	return (len);
}

size_t	ft_strlen(const char *str)
{
	char	*end;

	end = (char*)str;
	while (*end)
		end++;
	return ((const char*)end - str);
}

static int		line_joinbuff(char **line, char *buffer, size_t len)
{
	size_t	newlen;
	char	*newline;
	char	*newlinecpy;
	char	*linecpy;

	newlen = ft_strlen(*line) + len + 1;
	if (!(newline = static_cast<char *>(malloc(newlen * sizeof(char)))))
	{
		free(*line);
		*line = NULL;
		return (0);
	}
	newlinecpy = newline;
	linecpy = *line;
	while (*linecpy)
		*(newlinecpy++) = *(linecpy++);
	while (len && len--)
		*(newlinecpy++) = *(buffer++);
	*newlinecpy = '\0';
	free(*line);
	*line = newline;
	return (1);
}

static int		joinline(char **line, char *buffer, ssize_t buffer_size)
{
	ssize_t	bufflen;

	bufflen = buff_curnt_line_len(buffer, buffer_size);
	if (bufflen)
		if (!line_joinbuff(line, buffer, bufflen))
			return (-1);
	return (buff_strip_used(bufflen, buffer, buffer_size));
}

int		get_next_line(int fd, char **line, char *buffer, ssize_t buffer_size)
{
	int			prefill;
	int			eof;
	int			join_state;

	if (fd < 0 || !line || !buffer_size
			|| !(*line = static_cast<char *>(malloc(sizeof(char)))))
		return (-1);
	**line = '\0';
	eof = 1;
	join_state = 1;
	prefill = buffer_size && *buffer != 0;
	while (prefill || (join_state && (eof = read(fd, buffer, buffer_size)) > 0))
	{
		if ((join_state = joinline(line, buffer, buffer_size)) == -1)
			return (-1);
		prefill = 0;
	}
	if (eof == -1)
	{
		free(*line);
		*line = NULL;
	}
	return (eof > 0 ? 1 : eof);
}

int		get_next_line_string(int fd, std::string &line, char *buff, ssize_t size)
{
	int		ret;
	char	*c_line;

	ret = get_next_line(fd, &c_line, buff, size);
	if (c_line)
	{
		line = std::string(c_line);
		free(c_line);
	}

	return (ret);
}

#endif
