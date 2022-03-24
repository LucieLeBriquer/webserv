NAME = webserv

OBJS = $(SRCS:.cpp=.o)

CC = clang++ -Wall -Wextra -Werror -std=c++98 #-Wc++11-extensions #

RM = rm -rf

.cpp.o:
	@$(CC) -c $< -o ${<:.cpp=.o}

all: ${NAME}

$(NAME): ${OBJS}
	@${CC} -I./* ${OBJS} -o ${NAME}

clean:
	@${RM} ${OBJS}
	@echo "$(_RED) Everything was cleaned. $(_END)"

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re