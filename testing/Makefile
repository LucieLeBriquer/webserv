CC			= clang
RM			= rm -rf
NAME_C		= ./client
NAME_S		= ./server

SRCS_C		= TCPclient.c
SRCS_S		= TCPserver.c

_COLOR		= \033[32m
_BOLDCOLOR	= \033[32;1m
_RESET		= \033[0m
_CLEAR		= \033[0K\r\c
_OK			= [\033[32mOK\033[0m]

all			: $(NAME_C) $(NAME_S)

$(NAME_C)	: $(SRCS_C)
			@clang $(SRCS_C) -o $(NAME_C)

$(NAME_S)	: $(SRCS_S)
			@clang $(SRCS_S) -o $(NAME_S)

clean		:
			@$(RM) $(NAME_C) $(NAME_S)

fclean		: clean

re			: fclean all

.PHONY		: all clean fclean re
