/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:09:24 by julcleme          #+#    #+#             */
/*   Updated: 2025/12/03 18:00:32 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>
#include "get_next_line.h"

int	load_map(char *path, window *win)
{
	int		fd;
	int		i;
	char	*line;

	win->map.size.x = 0;
	win->map.size.y = 0;
	win->ceiling_color = 0x000000;
	win->floor_color = 0x000000;
	win->map.map = malloc(sizeof(char *) * 0);
    if (!win->map.map)
		return 1;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	while ((line = get_next_line(fd)) != NULL)
	{
		if (line[0] == '\0')
        {
			free(line);
			continue;
        }
		else if (!strncmp(line, "NO ", 2) || !strncmp(line, "SO ", 2) || !strncmp(line, "WE ", 2) || !strncmp(line, "EA ", 2))
		{
			i = 3;
			win->textures[0].img = mlx_xpm_file_to_image(win->mlx, "./textures/texture.xpm",
															&win->textures[0].width,
															&win->textures[0].height);
			win->textures[0].addr = mlx_get_data_addr(win->textures[0].img, 
															&win->textures[0].bpp, 
															&win->textures[0].line_len, 
															&win->textures[0].endian);
		}
		else if (!strncmp(line, "F ", 2) || !strncmp(line, "C ", 2))
		{
			i = 2;
			int start = i;
			int color = 0;
			int count = 0;

			while (line[i] && count < 3)
			{
				if (line[i] == ',' || line[i] == '\n')
				{
					color = (color << 8) | atoi(line + start);
					count++;
					start = i + 1;
				}
				i++;
			}
			if (count < 3)
				color = (color << 8) | atoi(line + start);

			if (line[0] == 'F')
				win->floor_color = color;
			else
				win->ceiling_color = color;
		}
		else
		{
			if (strlen(line) > win->map.size.x)
				win->map.size.x = strlen(line);			
			win->map.map = realloc(win->map.map, sizeof(char *) * (win->map.size.y + 1));
			if (!win->map.map)
				return (0);
			win->map.map[(int)win->map.size.y] = malloc(sizeof(char) * win->map.size.x + 1);
			if (!win->map.map[(int)win->map.size.y])
				return (0);
			i = -1;
			while (line[++i])
			{
				if (line[i] == 'N' || line[i] == 'E' || line[i] == 'W' || line[i] == 'S')
				{
					if (line[i] == 'N')
						win->player.direction = M_PI / 2;
					else if (line[i] == 'E')
						win->player.direction = M_PI;
					else if (line[i] == 'W')
						win->player.direction = 0;
					else
						win->player.direction = -M_PI / 2;
					win->player.pos.x = i;
					win->player.pos.y = win->map.size.y;
				}
				else
					win->map.map[(int)win->map.size.y][i] = line[i];
			}
			win->map.size.y++;
		}
		free(line);
	}
	close(fd);
	return (0);
}

void	unload_map(window *win)
{
	int	i;

	i = 0;
	while (i < win->map.size.y)
		free(win->map.map[i]);
	free(win->map.map);
}
