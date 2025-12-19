/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:06:51 by julcleme          #+#    #+#             */
/*   Updated: 2025/12/19 19:24:33 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "mlx.h"
#include "mlx_int.h"
#include <string.h>
#include "stdio.h"
#include <sys/time.h>

#define RENDER_DISTANCE 10.0f
#define FOV				60.0f
#define SCREEN_WIDTH	1900
#define SCREEN_HEIGHT	1500
#define RAY_STEP		0.01f
#define MOVE_SPEED		1.0
#define ROT_SPEED		2.0
#define SCALE_FACTOR	0.5

#define KEY_W			122
#define KEY_A			113
#define KEY_S			115
#define KEY_D			100
#define KEY_ESC			65307
#define KEY_RIGHT_ARROW	65363
#define KEY_DOWN_ARROW	65364
#define KEY_LEFT_ARROW	65361
#define KEY_UP_ARROW	65362

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_player
{
	t_point	pos;
	double	direction;
}	t_player;

typedef struct s_map
{
	t_point	size;
	char	**map;
}	t_map;

typedef struct s_texture
{
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
	t_point		size;
	void		*img;
	void		*addr;
}	t_texture;

typedef struct window
{
	void		*mlx;
	void		*win;
	void		*img;
	void		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	t_player	player;
	char		keys[65536];
	t_map		map;
	t_point		spawn_point;
	int			floor_color;
	int			ceiling_color;
	t_texture	texture_n;
	t_texture	texture_s;
	t_texture	texture_e;
	t_texture	texture_w;
	struct timeval last_time;
}	window;

int		load_map(char *path, window *win);
void	unload_map(window *win);

#endif