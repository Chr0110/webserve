NAME	=	webserv
RM		=	rm -f
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
SRCS	=	webserv.cpp

all: $(NAME)

$(NAME): $(SRCS)
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	@$(RM) $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re