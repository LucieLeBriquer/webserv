/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:28:17 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/30 16:19:22 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/engine.hpp"

int		addCreateSocketEpoll(Socket *sock, struct epoll_event ev, int *epollfd, std::map<int, const char *> map)
{
	if ((*epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		perror("epoll_create()");
		return -1;
	}
	
	for (int i = 0; i < (int)map.size(); i++)
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

int		socketMatch(int fde, Socket *sock)
{
	for (int i = 0; i < sock->getSocketNbr(); i++)
	{
		if (fde == sock->getSocket(i))
			return i;
	}
	return -1;
}

int		init_epoll(Socket *sock, std::map<int, const char *> map)
{
	struct epoll_event	ev, events[MAX_EVENTS];
	char				buf[30000] = {0};
	int					byteCount, nfds, epollfd, i;
	
	if (!addCreateSocketEpoll(sock, ev, &epollfd, map))
		return -1;

	while (1)
	{
		std::cout << std::endl;
		std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
		std::cout << std::endl;
		
		if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) < 0)
		{
			perror("epoll_wait()");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; n++)
		{
			std::cout << "nfds = " << nfds << *sock << " events[n].data.fd = " << events[n].data.fd << std::endl;
			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
			{
				/*An error on the fd or the socket is not ready*/
				std::cout << "Epoll error, events = " << events[n].events << std::endl;
				close(events[n].data.fd);
				continue ;
			}
			else if ((i = socketMatch(events[n].data.fd, sock)) >= 0)
			{
				/*We have one or more incoming connections, notifications on the listening socket*/
				while (1)
				{
					struct sockaddr	in_addr;
					socklen_t		in_addr_len = sizeof(in_addr);

					if ((sock->getConnSock(i) = accept(sock->getSocket(i), (struct sockaddr *) &in_addr, &in_addr_len)) < 0)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
						{
							std::cout << "we already processed all incoming connections" << std::endl;
							break ;
						}
						else
						{
							perror("accept()");
							exit(EXIT_FAILURE);
						}
					}
					if (setsocknonblock(sock->getConnSock(i)) < 0)
						exit(EXIT_FAILURE);
					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = sock->getConnSock(i);
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock->getConnSock(i), &ev) < 0)
					{
						perror("epoll_ctl: sock->getConnSock(i)");
						exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
				/*Data waiting to be read on our waiting fd*/
				memset(buf, 0, sizeof(buf));
				while (1)
				{
					byteCount = recv(events[n].data.fd, buf, sizeof(buf), MSG_DONTWAIT);
					if (byteCount == 0)
					{
						epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
						break ;
					}
					else if (byteCount < 0)
					{
						close(events[n].data.fd);
						break ;
					}
					else
					{
						std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";

						std::cout << "get " << byteCount << " bytes of content from " << events[n].data.fd << " [ " << buf << " ] " << std::endl;
						if (send(events[n].data.fd, hello.c_str(), hello.size(), MSG_DONTWAIT) < 0)
						{
							perror("send()");
							exit(EXIT_FAILURE);
						}
						std::cout << "sending data to " << events[n].data.fd << std::endl;
						close(events[n].data.fd);
					}
				}
			}
		}
	}
	close(epollfd);
	for (int i = 0; i < sock->getSocketNbr(); i++)
		close(sock->getSocket(i));
}