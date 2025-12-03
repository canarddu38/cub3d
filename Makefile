NAME		= cub3d
MLX_PATH	= minilibx-linux
MLX			= $(MLX_PATH)/libmlx_Linux.a
CFLAGS		= -Wall -Werror -Wextra -I$(MLX_PATH) -IGNL -g3
CPPFLAGS	= -lXext -lX11 -lm -MMD -MP
CFILES		=	main.c \
				parser.c \
				GNL/get_next_line.c \
				GNL/get_next_line_utils.c
OFILES		= $(CFILES:.c=.o)
D_FILES		= $(OFILES:.o=.d)

all: $(NAME)

$(MLX):
	$(MAKE) -C $(MLX_PATH) all

$(NAME): $(MLX) $(OFILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(OFILES) $(MLX) -lXext -lX11 -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) -rf $(OFILES) $(D_FILES)

fclean: clean
	$(RM) -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean

-include: $(D_FILES)