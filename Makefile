BLACK=\033[0;30m
RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
WHITE=\033[0;37m
END=\033[0m

NAME = webserv

SRCS = server.cpp \
		get_methods.cpp \
		error4xx.cpp \
		strManip.cpp

OBJS = $(SRCS:.cpp=.o)

CC = clang++ -Wall -Wextra -Werror -std=c++98 #-Wc++11-extensions #

RM = rm -rf

.cpp.o:
	@$(CC) -c $< -o ${<:.cpp=.o}

all: ${NAME}

$(NAME): ${OBJS}
	@${CC} -I./* ${OBJS} -o ${NAME}
	@echo "[$(GREEN)ok$(END)] Compilation"

clean:
	@${RM} ${OBJS}
	@echo "$(RED) Everything was cleaned. $(END)"

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re