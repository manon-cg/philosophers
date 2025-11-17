NAME = philo

SRC = 	main.c \
		utils.c philo_actions.c cleanup.c init.c fork_utils.c \
		continuation_checks.c \
testing_only.c

HEADER = philo.h

OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g
# FLAGS = -Wall -Wextra -Werror -g -fsanitize=thread

all : $(NAME)

$(NAME) : $(OBJ)
	cc $(FLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(HEADER)
	cc $(FLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re