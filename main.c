/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 09:56:28 by julcleme          #+#    #+#             */
/*   Updated: 2025/12/19 19:23:48 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	cast_ray(window *win, t_point *local_hit_point, double angle)
{
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	ray_dir_x;
	double	ray_dir_y;

	map_x = (int)win->player.pos.x;
	map_y = (int)win->player.pos.y;
	ray_dir_x = cos(angle);
	ray_dir_y = sin(angle);
	delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
	delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
	hit = 0;
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (win->player.pos.x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - win->player.pos.x) * delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (win->player.pos.y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - win->player.pos.y) * delta_dist_y;
	}
	while (hit == 0)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (map_x < 0 || map_x >= win->map.size.x || map_y < 0 || map_y >= win->map.size.y)
			return (-1.0f);
		if (win->map.map[map_y][map_x] == '1')
			hit = 1;
	}
	if (side == 0)
		perp_wall_dist = (side_dist_x - delta_dist_x);
	else
		perp_wall_dist = (side_dist_y - delta_dist_y);
	if (perp_wall_dist > RENDER_DISTANCE)
		return (-1.0f);
	if (side == 0)
	{
		local_hit_point->x = 0.0;
		local_hit_point->y = win->player.pos.y + perp_wall_dist * ray_dir_y;
		local_hit_point->y -= floor(local_hit_point->y);
	}
	else
	{
		local_hit_point->x = 1.0;
		local_hit_point->y = win->player.pos.x + perp_wall_dist * ray_dir_x;
		local_hit_point->y -= floor(local_hit_point->y);
	}
	return (perp_wall_dist);
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

int move_player(window *win, double delta_time)
{
    double nx, ny;
	double move_step = MOVE_SPEED * delta_time;

    if (win->keys[KEY_A])
	{
		nx = win->player.pos.x + cos(win->player.direction - M_PI / 2) * move_step;
    	ny = win->player.pos.y + sin(win->player.direction - M_PI / 2) * move_step;
	}
    if (win->keys[KEY_D])
	{
		nx = win->player.pos.x + cos(win->player.direction + M_PI / 2) * move_step;
    	ny = win->player.pos.y + sin(win->player.direction + M_PI / 2) * move_step;
	}
    if (win->keys[KEY_W])
    {
        nx = win->player.pos.x + cos(win->player.direction) * move_step;
        ny = win->player.pos.y + sin(win->player.direction) * move_step;
    }
    if (win->keys[KEY_S])
    {
        nx = win->player.pos.x - cos(win->player.direction) * move_step;
        ny = win->player.pos.y - sin(win->player.direction) * move_step;
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
	if (!t || t->height <= 0 || h <= 0)
		return;

	int start = SCREEN_HEIGHT / 2 - h / 2;
	int end   = SCREEN_HEIGHT / 2 + h / 2;

	if (start < 0) start = 0;
	if (end >= SCREEN_HEIGHT) end = SCREEN_HEIGHT - 1;

	double texture_step = (double)t->height / (double)h;
	double texture_y = (start - (SCREEN_HEIGHT / 2 - h / 2)) * texture_step;

	char *dst = win->addr + (start * win->line_len + x * (win->bpp / 8));
	int line_len = win->line_len;
	int bpp_stride = win->bpp / 8;

	char *dst_ceil = win->addr + (x * bpp_stride);
	for (int y = 0; y < start; y++)
	{
		*(unsigned int *)dst_ceil = win->ceiling_color;
		dst_ceil += line_len;
	}

	for (int y = start; y <= end; y++)
	{
		int ty = (int)texture_y;
		if (ty < 0) ty = 0;
		if (ty >= t->height) ty = t->height - 1;

		char *color = t->addr + (ty * t->line_len + c * (t->bpp / 8));
		*(unsigned int *)dst = *(unsigned int *)color;
		texture_y += texture_step;
		dst += line_len;
	}

	char *dst_floor = win->addr + ((end + 1) * line_len + x * bpp_stride);
	for (int y = end + 1; y < SCREEN_HEIGHT; y++)
	{
		*(unsigned int *)dst_floor = win->floor_color;
		dst_floor += line_len;
	}
}


void	put_line(window *win, int x, int h, int color)
{
	if (x < 0 || x >= SCREEN_WIDTH || h <= 0)
        return;
	int	start = SCREEN_HEIGHT / 2 - h / 2;
	int	end = SCREEN_HEIGHT / 2 + h / 2;
	
	if (start < 0) start = 0;
	if (end >= SCREEN_HEIGHT) end = SCREEN_HEIGHT - 1;

	int line_len = win->line_len;
	int bpp_stride = win->bpp / 8;
	char *dst = win->addr + (x * bpp_stride);

	for (int y = 0; y < start; y++)
	{
		*(unsigned int *)dst = win->ceiling_color;
		dst += line_len;
	}

	for (int y = start; y <= end; y++)
	{
		*(unsigned int *)dst = color;
		dst += line_len;
	}

	for (int y = end + 1; y < SCREEN_HEIGHT; y++)
	{
		*(unsigned int *)dst = win->floor_color;
		dst += line_len;
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
	float			angle_step;
	struct timeval	current_time;
	double			delta_time;

	gettimeofday(&current_time, NULL);
	delta_time = (current_time.tv_sec - win->last_time.tv_sec) + 
				 (current_time.tv_usec - win->last_time.tv_usec) / 1000000.0;
	win->last_time = current_time;

	move_player(win, delta_time);
	x = 0;
	angle_step = (FOV / SCREEN_WIDTH) * M_PI / 180.0;
	rayAngle = win->player.direction - ((FOV / 2.0) * M_PI / 180.0);
	while (x < SCREEN_WIDTH)
	{
        distance = cast_ray(win, &local_hit_point, rayAngle);
        if (distance > 0)
			height = (int)((SCREEN_HEIGHT / distance) * SCALE_FACTOR);
		else
			height = 1;
		if (local_hit_point.x <= RAY_STEP)
		{
			color = local_hit_point.y * win->texture_n.width;
			put_image_line(win, &win->texture_n, color, x, height);
		}
		else
			put_line(win, x, height, 0x1e1e1e);
		rayAngle += angle_step;
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

	win.mlx = mlx_init();
	win.win = mlx_new_window(win.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	win.img = mlx_new_image(win.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	win.addr = mlx_get_data_addr(win.img, &win.bpp, &win.line_len, &win.endian);

	load_map(argv[1], &win);

	mlx_mouse_hide(win.mlx, win.win);
	mlx_mouse_move(win.mlx, win.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	
	mlx_hook(win.win, 2, 1L<<0, key_press, &win);
	mlx_hook(win.win, 3, 1L<<1, key_release, &win);
    mlx_hook(win.win, 17, 0, close_window, &win);

	mlx_hook(win.win, 6, 1L << 6, mouse_move, &win);

	gettimeofday(&win.last_time, NULL);
    mlx_loop_hook(win.mlx, render_frame, &win);
	mlx_loop(win.mlx);
	unload_map(&win);
	mlx_mouse_show(win.mlx, win.win);
	return (0);
}
