CC			= c++ -Wall -Wextra -Werror -std=c++98
RM			= rm -rf
NAME		= ./webserv
NAME_SHORT	= webserv

INCS_DIR	= ./includes/
MAIN_INC	= -I$(INCS_DIR)
INCS		= $(addprefix $(INCS_DIR), Block.hpp \
			Client.hpp \
			Config.hpp \
			engine.hpp \
			httpHeader.hpp \
			httpRequest.hpp \
			httpResponse.hpp \
			Location.hpp \
			mime.hpp \
			Server.hpp \
			Socket.hpp \
			Status.hpp \
			usefull.hpp)

SRCS_DIR 	= ./sources/
SRCS		= $(addprefix $(SRCS_DIR), main.cpp \
			engine/cgi.cpp \
			engine/send.cpp \
			engine/epoll.cpp \
			engine/env.cpp \
			engine/class/Client.cpp \
			engine/class/Socket.cpp \
			engine/message.cpp \
			engine/utils.cpp \
			request/httpRequest.cpp \
			request/httpHeader.cpp \
			response/default.cpp \
			response/httpResponse.cpp \
			response/Status.cpp \
			response/mime.cpp \
			parsing/Server.cpp \
			parsing/Location.cpp \
			parsing/Config.cpp \
			parsing/Block.cpp \
			parsing/usefull.cpp)

OBJS_DIR	= ./objects/
OBJS		= $(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_DIR)%.o)

SUB_DIRS	= $(addprefix $(OBJS_DIR), parsing engine request response engine/class)

UP_DIR 		= ./html/uploads

_COLOR		= \033[32m
_BOLDCOLOR	= \033[32;1m
_RESET		= \033[0m
_CLEAR		= \033[0K\r\c
_OK			= [\033[32mOK\033[0m]

$(OBJS_DIR)%.o	: $(SRCS_DIR)%.cpp
			@mkdir -p $(OBJS_DIR) $(SUB_DIRS) $(UP_DIR)
			@echo "[..] $(NAME_SHORT)... compiling $*.cpp\r\c"
			@$(CC) $(MAIN_INC) -c $< -o $@
			@echo "$(_CLEAR)"

all			: $(NAME)

$(NAME)		: $(OBJS) $(INCS)
			@$(CC) $(OBJS) $(MAIN_INC) -o $(NAME)
			@echo "$(_OK) $(NAME_SHORT) compiled"

clean		:
ifeq ($(OBJS_DIR), ./)
			@$(RM) $(OBJS)
else
			@$(RM) $(OBJS_DIR)
endif
			@$(RM) $(UP_DIR)

fclean		: clean
			@$(RM) $(NAME)

re			: fclean all

.PHONY		: all clean fclean re

