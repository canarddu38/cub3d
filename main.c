/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 09:56:28 by julcleme          #+#    #+#             */
/*   Updated: 2025/12/03 18:09:13 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	cast_ray(window *win, t_point *local_hit_point, double angle)
{
	double	distance = 0.0f;
	t_point	ray = {
		.x = win->player.pos.x,
		.y = win->player.pos.y
	};
	t_point	direction = {	
		.x = (double)cos(angle) * RAY_STEP,
		.y = (double)sin(angle) * RAY_STEP
	};
	while (1)
	{
		ray.x += direction.x;
		ray.y += direction.y;
		distance += RAY_STEP;
		if ((int)ray.x < 0 || (int)ray.x >= win->map.size.x || (int)ray.y < 0 || (int)ray.y >= win->map.size.y || distance > RENDER_DISTANCE)
			return (-1.0f);
		local_hit_point->x = ray.x - (int)ray.x;
		local_hit_point->y = ray.y - (int)ray.y;
		if (win->map.map[(int)ray.y][(int)ray.x] == '1')
			break;
	}
	return (distance);
}

int	close_window(void *mlx)
{
	(void)mlx;
	exit(0);
	return (0);
}

int	mouse_move(int x, int y, window *win)
{
	int		delta;

	(void)y;
	delta =  (SCREEN_WIDTH / 2) - x;
	if (delta > 5)
		win->player.direction -= 0.05f;
	else if (delta < -5)
		win->player.direction += 0.05f;
	mlx_mouse_move(win->mlx, win->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	return 0;
}

int move_player(window *win)
{
    double nx, ny;

    if (win->keys[KEY_A])
	{
		nx = win->player.pos.x + cos(win->player.direction - M_PI / 2) * MOVE_SPEED;
    	ny = win->player.pos.y + sin(win->player.direction - M_PI / 2) * MOVE_SPEED;
	}
    if (win->keys[KEY_D])
	{
		nx = win->player.pos.x + cos(win->player.direction + M_PI / 2) * MOVE_SPEED;
    	ny = win->player.pos.y + sin(win->player.direction + M_PI / 2) * MOVE_SPEED;
	}
    if (win->keys[KEY_W])
    {
        nx = win->player.pos.x + cos(win->player.direction) * MOVE_SPEED;
        ny = win->player.pos.y + sin(win->player.direction) * MOVE_SPEED;
    }
    if (win->keys[KEY_S])
    {
        nx = win->player.pos.x - cos(win->player.direction) * MOVE_SPEED;
        ny = win->player.pos.y - sin(win->player.direction) * MOVE_SPEED;
    }
    if (win->map.map[(int)ny][(int)nx] != '1')
    {
        win->player.pos.x = nx;
    	win->player.pos.y = ny;
    }
    if (win->player.direction < 0)
        win->player.direction += 2 * M_PI;
    if (win->player.direction >= 2 * M_PI)
        win->player.direction -= 2 * M_PI;
    return (0);
}

void put_image_line(window *win, t_texture *t, int c, int x, int h)
{
	int	start = SCREEN_HEIGHT / 2 - h / 2;
	int	end = SCREEN_HEIGHT / 2 + h / 2;
	
	for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
		if (y >= start && y <= end)
			*((unsigned int *)win->addr + (y * win->line_len + x * (win->bpp / 8))) = 
								*((unsigned int *)(t->addr + (y * t->line_len + c * (t->bpp / 8))));
    }
}

void	put_line(window *win, int x, int h, int color)
{
	if (x < 0 || x >= SCREEN_WIDTH || h <= 0)
        return;
	int	start = SCREEN_HEIGHT / 2 - h / 2;
	int	end = SCREEN_HEIGHT / 2 + h / 2;
	char *dst;

	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		dst = win->addr + (y * win->line_len + x * (win->bpp / 8));
		if (y >= start && y <= end)
			*(unsigned int *)dst = color;
		else
		{
			if (y > SCREEN_HEIGHT / 2)
				*(unsigned int *)dst = win->floor_color;
			else
				*(unsigned int *)dst = win->ceiling_color;
		}
			
	}
}

int	render_frame(window *win)
{
	double distance;
	t_point			local_hit_point;
	double			rayAngle;
	int				x;
	int				height;
	int				color;
	unsigned char	c;
	
	move_player(win);
	distance = 0;
	x = 0;
	height = 0.0f;
	c = 0;
	color = 0;
	while (x < SCREEN_WIDTH)
	{
		rayAngle = win->player.direction - (FOV / 2.0) * M_PI / 180.0 + (x * (FOV / SCREEN_WIDTH) * M_PI / 180.0);
        distance = cast_ray(win, &local_hit_point, rayAngle);
        if (distance > 0)
			height = (int)((SCREEN_HEIGHT / distance) * SCALE_FACTOR);
		else
			height = 1;
		//int base = 255;
		if (local_hit_point.x <= RAY_STEP)
			color = 0x00ff00;
		else if (local_hit_point.x >= 1 - RAY_STEP)
			color = 0x0000ff;
		else if (local_hit_point.y <= RAY_STEP)
			color = 0x00ffff;
		else if (local_hit_point.y >= 1 - RAY_STEP)
			color = 0xff00ff;
		else
			color = 0xff0000;
		//c = base - (int)(base * distance / RENDER_DISTANCE);
		//color = (c << 16) | (c << 8) | c;
		//put_line(win, x, height, color);
		put_image_line(win, &win->textures[0], c, x, height);
        x++;
	}
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (0);
}

int key_press(int key, window *win)
{
    if (key >= 0 && key < 65536)
        win->keys[key] = 1;
    if (key == KEY_ESC)
	{
		mlx_mouse_show(win->mlx, win->win);
		exit(0);
	}
    return 0;
}

int	key_release(int key, window *win)
{
    if (key >= 0 && key < 65536)
        win->keys[key] = 0;
    return (0);
}

int	main(int argc, char **argv)
{
	window		win;

	if (argc <= 1)
		return (1);
	memset(win.keys, 0, sizeof(win.keys));
	load_map(argv[1], &win);

	win.mlx = mlx_init();
	win.win = mlx_new_window(win.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cube3d");
	win.img = mlx_new_image(win.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	win.addr = mlx_get_data_addr(win.img, &win.bpp, &win.line_len, &win.endian);

	mlx_mouse_hide(win.mlx, win.win);
	mlx_mouse_move(win.mlx, win.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	
	mlx_hook(win.win, 2, 1L<<0, key_press, &win);
	mlx_hook(win.win, 3, 1L<<1, key_release, &win);
    mlx_hook(win.win, 17, 0, close_window, &win);

	mlx_hook(win.win, 6, 1L << 6, mouse_move, &win);

    mlx_loop_hook(win.mlx, render_frame, &win);
	mlx_loop(win.mlx);
	unload_map(&win);
	mlx_mouse_show(win.mlx, win.win);
	return (0);
}
