NAME	= philo
CC		= cc 
CFLAGS	= -Werror -Wall -Wextra -pthread
#CFLAGS	+= -g -O0 -fsanitize=thread
#CFLAGS	+= -g -O0 -fsanitize=address

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main.c \
			parsing.c \
			check_end.c \
			time.c \
			philosopher.c \
			init.c \
			output.c \
			utils.c \
			exit.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

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

.PHONY: all re clean fclean