/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:28:17 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/13 12:42:49 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void printStatus(int i, int nfds, Socket *sock, int fde)
{
	switch (i)
	{
	case 1:
	{
		std::cout << std::endl;
		std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
		std::cout << std::endl;
		break;
	}
	case 2:
	{
		std::cout << "nfds = " << nfds << *sock << " events[n].data.fd = " << fde << std::endl;
		break;
	}
	}
}

int socketMatch(int fde, Socket *sock)
{
	for (int i = 0; i < sock->getSocketNbr(); i++)
	{
		if (fde == sock->getSocket(i))
			return i;
	}
	return -1;
}

Socket *initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i)
{
	while (1)
	{
		struct sockaddr in_addr;
		socklen_t in_addr_len = sizeof(in_addr);

		if ((sock->modConnSock(i) = accept(sock->getSocket(i), (struct sockaddr *)&in_addr, &in_addr_len)) < 0)
		{
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
			{
				std::cout << "we already processed all incoming connections" << std::endl;
				break;
			}
			else
			{
				perror("accept()");
				return NULL;
			}
		}
		if (setsocknonblock(sock->getConnSock(i)) < 0)
			return NULL;
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = sock->getConnSock(i);
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock->getConnSock(i), &ev) < 0)
		{
			perror("epoll_ctl: sock->getConnSock(i)");
			return NULL;
		}
	}
	std::cout << "sock is returned" << std::endl;
	return sock;
}

int addCreateSocketEpoll(Socket *sock, struct epoll_event ev, int *epollfd, const Config config)
{
	if ((*epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		perror("epoll_create()");
		return -1;
	}

	for (int i = 0; i < (int)config.getServers().size(); i++)
	{
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = sock->getSocket(i);
		if (epoll_ctl(*epollfd, EPOLL_CTL_ADD, sock->getSocket(i), &ev) < 0)
		{
			perror("epoll_ctl: sock.getSocket(i)");
			return -1;
		}
	}
	return 1;
}

int initEpoll(Socket *sock, const Config config)
{
	struct epoll_event ev, events[MAX_EVENTS];
	int nfds = 0;
	int epollfd, i;

	if (!addCreateSocketEpoll(sock, ev, &epollfd, config))
		return (ERR);

	while (1)
	{
		printStatus(1, nfds, sock, events[MAX_EVENTS - 1].data.fd);

		if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) < 0)
		{
			perror("epoll_wait()");
			return (ERR);
		}
		for (int n = 0; n < nfds; n++)
		{
			printStatus(2, nfds, sock, events[n].data.fd);
			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
			{
				std::cout << "Epoll error, events = " << events[n].events << std::endl;
				close(events[n].data.fd);
				continue;
			}
			else if ((i = socketMatch(events[n].data.fd, sock)) >= 0)
			{
				if ((sock = initConnection(sock, ev, epollfd, i)) == NULL)
					return (ERR);
			}
			else
			{
				if (requestReponse(epollfd, events[n].data.fd, sock, n))
					return (ERR);
			}
		}
	}
	close(epollfd);
	for (int i = 0; i < sock->getSocketNbr(); i++)
		close(sock->getSocket(i));
	return (OK);
}