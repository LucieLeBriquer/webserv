/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/06 15:15:30 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP

# include "Socket.hpp"

# include <map>
# include <utility>
# include <sys/types.h>
# include <iostream>
# include <unistd.h>
# include <cstdio>
# include <sys/epoll.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <sstream>
# include <sys/types.h>
# include <sys/wait.h>

# define B_SIZE 2
# define MAX_EVENTS 10
# define BUFFER_SIZE 1024

int		setsocknonblock(int sock);
int		initSockets(Socket *sock, const Config config);
int		initEpoll(Socket *sock, const Config config);
int		socketMatch(int fde, Socket *sock);
Socket	*initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i);
int		requestReponse(int epollfd, int fde, char **env);

void	GetCGIfile(std::string *file, int *tot_size, char **env);

#endif
