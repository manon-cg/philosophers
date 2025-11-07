NAME = philo

SRC = main.c utils.c \
testing_only.c

HEADER = philo.h

OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

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