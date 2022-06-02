/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:28:17 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/27 13:58:29 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static void	initEpollEvent(struct epoll_event *ev, uint32_t flag, int fd)
{
	memset(ev, 0, sizeof(*ev));
	ev->events = flag;
	ev->data.fd = fd;
}

int	initConnection(Socket &sock, int i)
{
	struct epoll_event	ev;
	struct sockaddr 	in_addr;
	socklen_t			in_addr_len = sizeof(in_addr);
	int					newFd;

	if ((newFd = accept(sock.getSocket(i), (struct sockaddr *)&in_addr, &in_addr_len)) < 0)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
			if (LOG_LEVEL >= LVL_INFO)
				std::cout << "we already processed all incoming connections" << std::endl;
		}
		else
		{
			perror("accept()");
			return (ERR);
		}
	}
	if (setsocknonblock(newFd) < 0)
		return (ERR);
	
	initEpollEvent(&ev, EPOLLIN, newFd);
	sock.addConnection(newFd, i);

	if (LOG_LEVEL >= LVL_INFO)
	{
		std::cout << YELLOW << "[Accept]" << END << " connection on socket " + toString(i) + " at " + sock.getHost(i) + ":" + sock.getPort(i) << std::endl;
		std::cout << GRAY << std::setw(52) << "socket " + toString(newFd) + " created to communicate" << END << std::endl;
	}
	if (epoll_ctl(sock.getEpollFd(), EPOLL_CTL_ADD, newFd, &ev) < 0)
	{
		perror("epoll_ctl: sock.getConnSock(i)");
		return (ERR);
	}
	return (OK);
}

int addCreateSocketEpoll(Socket &sock)
{
	struct epoll_event ev;

	sock.setEpollFd(epoll_create(MAX_EVENTS));
	if (sock.getEpollFd() < 0)
	{
		perror("epoll_create()");
		return (ERR);
	}
	
	for (size_t i = 0; i < sock.getNumberListen(); i++)
	{
		initEpollEvent(&ev, EPOLLIN, sock.getSocket(i));
		if (epoll_ctl(sock.getEpollFd(), EPOLL_CTL_ADD, sock.getSocket(i), &ev) < 0)
		{
			perror("epoll_ctl: sock.getSocket(i)");
			return (ERR);
		}
	}

	initEpollEvent(&ev, EPOLLIN, 0);
	if (epoll_ctl(sock.getEpollFd(), EPOLL_CTL_ADD, 0, &ev) < 0)
	{
		perror("epoll_ctl: stdin");
		return (ERR);
	}
	return (OK);
}

int	waitEpoll(Socket &sock)
{
	struct epoll_event	events[MAX_EVENTS];
	int 				nfds = 0;
	int					i = 0;
	
	if (LOG_LEVEL >= LVL_INFO)
		std::cout << std::endl << BLUE << "++++++++++++ Waiting for new connection ++++++++++++" << END << std::endl << std::endl;

	if ((nfds = epoll_wait(sock.getEpollFd(), events, MAX_EVENTS, -1)) < 0)
	{
		perror("epoll_wait()");
		return (ERR);
	}
	
	for (int n = 0; n < nfds; n++)
	{
		if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
		{
			std::cerr << "epoll error, events = " << events[n].events << std::endl;
			close(events[n].data.fd);
			return (OK);
		}
		else if ((i = sock.socketMatch(events[n].data.fd)) >= 0)
		{
			if (initConnection(sock, i))
				return (ERR);
		}
		else if (events[n].data.fd == 0)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return (ERR);
		}
		else
		{
			if (requestReponse(events[n].data.fd, sock))
				return (ERR);
		}
	}
	return (OK);
}

int initEpoll(Socket &sock)
{
	if (addCreateSocketEpoll(sock))
		return (ERR);

	std::cout << GREEN << "[Start]" << END " webserv..." << std::endl;
	while (waitEpoll(sock) != ERR)
		;
	std::cout << RED << "[Close]" << END << " webserv..." << std::endl;
	close(sock.getEpollFd());
	for (int i = 0; i < sock.getSocketNbr(); i++)
		close(sock.getSocket(i));
	return (OK);
}
