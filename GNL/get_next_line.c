/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:53:52 by julcleme          #+#    #+#             */
/*   Updated: 2025/11/16 00:46:35 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*move_buffer(char buffer[BUFFER_SIZE + 1], char **output, size_t i)
{
	*output = ft_strcat_realloc(*output, buffer, i + 1);
	ft_memmove(buffer, buffer + i + 1, BUFFER_SIZE - i);
	if (i == BUFFER_SIZE - 1)
		clear_buffer(buffer);
	return (*output);
}

static size_t	count_new_line(char buffer[BUFFER_SIZE + 1], size_t *new_line)
{
	size_t	i;

	i = 0;
	*new_line = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		*new_line = 1;
	return (i);
}

char	*get_next_line(int fd)
{
	static char	buffer[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*output;
	size_t		i;
	size_t		new_line_found;

	output = 0;
	if (fd < 0 || fd > FOPEN_MAX || BUFFER_SIZE < 0)
		return (0);
	i = count_new_line(buffer[fd], &new_line_found);
	if (new_line_found && buffer[fd][i + 1] != '\0')
		return (move_buffer(buffer[fd], &output, i));
	else if (buffer[fd][0] != '\0')
		output = ft_strcat_realloc(0, buffer[fd], i + 1);
	clear_buffer(buffer[fd]);
	while (read(fd, buffer[fd], BUFFER_SIZE) > 0)
	{
		i = count_new_line(buffer[fd], &new_line_found);
		move_buffer(buffer[fd], &output, i);
		if (new_line_found)
			break ;
		clear_buffer(buffer[fd]);
	}
	return (output);
}

/*
#include <fcntl.h>
int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		i;

	if (argc != 2)
		return (1);
	i = 0;
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)))
	{
		printf("'%s' : len=%zu\n", line, ft_strlen(line));
		free(line);
		i++;
	}
	close(fd);
	return (0);
}
*/
