NAME	= philo_bonus
CC		= cc
CFLAGS	= -Werror -Wall -Wextra -pthread
CFLAGS	+= -g

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main_bonus.c \
			parsing_bonus.c \
			routine_bonus.c \
			getters_bonus.c \
			init_bonus.c \
			time_bonus.c \
			utils_bonus.c \
			utils2_bonus.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

bonus: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean bonus