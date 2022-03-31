# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/28 15:23:15 by lpascrea          #+#    #+#              #
#    Updated: 2022/03/31 12:06:49 by lpascrea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLORS#
_END	= \033[0m
_GREEN	= \033[0;32m
_RED	= \033[0;31m
_BLUE	= \033[0;96m
_YELLOW	= \033[0;33m
_MAG 	= \033[0;35m
#------#

SRCS	=	main.cpp \
			./engine/Socket.cpp \
			./engine/init_sockets.cpp \
			./engine/epoll.cpp \
			./engine/request_reponse.cpp

OBJS	= $(SRCS:.cpp=.o)

CC		= c++

CFLAGS	= -Wall -Wextra -Werror -std=c++98

RM		= rm -rf

NAME	= webserv

all:		${NAME}
			@echo "$(_GREEN)webserv is ready $(_END)"

$(NAME):	${OBJS}
			${CC} ${CFLAGS} -I. -I./* ${OBJS} -o ${NAME}

%.o:		%.cpp
			${CC} ${CFLAGS} -o $@ -c $< -I. -I./*

clean:
			${RM} ${OBJS}
			@echo "$(_YELLOW) \".o\" files deleted $(_END)"

fclean:		clean
			${RM} ${NAME}
			@echo "$(_RED)webserv cleaned $(_END)"

re: 		fclean all

.PHONY: all clean fclean re