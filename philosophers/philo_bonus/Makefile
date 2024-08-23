CC		= cc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

CFLAGS	+=  -fsanitize=thread -g

SRC_DIR = src/
OBJ_DIR = obj/

SRC_FILES =	main \
			parsing \
			grim_reaper \
			time \
			philosopher \
			philosopher_utils \
			init \
			utils \
			ipc \
			output \
			cleanup \
			exit

SRC		= 	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean