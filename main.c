#include <unistd.h>
# include "mlx.h"
#include <math.h>
#include <stdlib.h>

#define RENDER_DISTANCE 10
#define FOV				60
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define KEY_ESC			65307
#define KEY_RIGHT_ARROW	65363
#define KEY_DOWN_ARROW	65364
#define KEY_LEFT_ARROW	65361
#define KEY_UP_ARROW	65362

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;


char	*map[6] = {
	"##########",
	"#        #",
	"#        #",
	"#    #   #",
	"#    #   #",
	"##########"
};

double	cast_ray(t_point *player, double angle)
{
	double	distance = 0.0f;
	t_point	ray = {player->x, player->y};
	t_point	direction = {cos(angle), sin(angle)};
	int	delta_dir = sqrt(direction.x * direction.x + direction.y * direction.y);

	while (distance < RENDER_DISTANCE)
	{
		ray.x += direction.x;
		ray.y += direction.y;
		if (map[ray.y][ray.x] == '#')
			return distance;
		distance += delta_dir;
	}
	return (-1);
}

int	close_window(void *mlx)
{
	(void)mlx;
	exit(0);
	return (0);
}

int	handle_key(int key)
{
	if (key == KEY_ESC)
		exit(0);
	//else if (key == KEY_LEFT_ARROW)
	//	direction[0] = -1;
	//else if (key == KEY_RIGHT_ARROW)
	//	direction[0] = 1;
	//else if (key == KEY_UP_ARROW)
	//	direction[1] = 1;
	//else if (key == KEY_DOWN_ARROW)
	//	direction[1] = -1;
	//else
	//	return (0);
	return (0);
}

void	put_line(window *img, int x, int h, int color)
{
	char	*dst;
	int		i;

	i = SCREEN_HEIGHT / 2 - h / 2;
	if (x < 0 || x >= SCREEN_WIDTH || h < 0 || h >= SCREEN_HEIGHT)
		return ;
	while (i < SCREEN_HEIGHT / 2 + h)
	{
		dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	}
	*(unsigned int *)dst = color;
}

typedef struct window
{
	void	*mlx;
	void	*win;
	void	*img;
	void	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	window;

int	main(void)
{
	window	window;
	t_point	player		= {5, 3};
	double	view_dir	= 0.0f;

	window.mlx = mlx_init();
	window.win = mlx_new_window(window.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cube3d");
	window.img = mlx_new_image(window.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	window.addr = mlx_get_data_addr(window.img, &window.bpp, &window.line_len, &window.endian);
	mlx_key_hook(window.win, handle_key, 0);
	mlx_hook(window.win, 17, 0, close_window, window.mlx);

	int	x = 0;
	double distance = 0;
	while (x < SCREEN_WIDTH)
	{
		distance = cast_ray(&player, (view_dir - FOV / 2) + (FOV / x));
		put_line(&window, x, 1 / distance, 0xffffff);
		x++;
	}

	mlx_loop(window.mlx);
	return 0;
}