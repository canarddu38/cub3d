NAME		= cube3d
MLX_PATH	= minilibx-linux
MLX			= $(MLX_PATH)/libmlx.a
CFLAGS		= -Wall -Werror -Wextra -I$(MLX_PATH) -MMD -MP
CFILES		= main.c
OFILES		= $(CFILES:.c=.o)
D_FILES		= $(C_FILES:.o=.d)

all: $(NAME)

$(MLX):
	$(MAKE) -C $(MLX_PATH) all

$(NAME): $(MLX) $(OFILES)
	$(CC) $^ -o $(@)

clean:
	$(RM) -rf $(OFILES)

fclean: clean
	$(RM) -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean

-include: $(D_FILES)