/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 10:17:14 by lpascrea         ###   ########.fr       */
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

# define PORT 8080
# define MAX_EVENTS 10

int		setsocknonblock(int sock);
int		init_sockets(Socket *sock, std::map<int, const char *> map);
int		init_epoll(Socket *sock, std::map<int, const char *> map);
int		addCreateSocketEpoll(Socket *sock, struct epoll_event ev, int *epollfd, std::map<int, const char *> map);
int		socketMatch(int fde, Socket *sock);
Socket	*initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i);
int		requestReponse(int epollfd, int *fde);

#endif
