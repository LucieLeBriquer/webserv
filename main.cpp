/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/30 11:01:49 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Config.hpp"
#include "includes/engine.hpp"

int	main(/*int argc, char **argv*/void)
{
	// Config	config(fileName(argc, argv));
	Socket		sock;
	
	/******instead of config lucie******/
	std::map<int, const char *> map;
	std::pair<int, const char *> conf1 = std::make_pair(8080, "127.0.0.1");
	// std::pair<int, const char *> conf2 = std::make_pair(8081, "127.0.0.2");
	map.insert(conf1);
	// map.insert(conf2);
	/***********************************/

	// if (!config.wellFormatted())
	// 	return (1);
	// std::cout << config << std::endl;

	if (!init_sockets(&sock, map))
		exit(EXIT_FAILURE);
	
	struct epoll_event	ev, events[MAX_EVENTS];
	char				buf[30000] = {0};
	int					byteCount, connSock, nfds, epollfd, n = 0;
	
	for (int i = 0; i < (int)map.size(); i++)
	{
		//epoll create
		if ((epollfd = epoll_create(MAX_EVENTS)) < 0)
		{
			perror("epoll_create()");
			exit(EXIT_FAILURE);
		}

		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = sock.getSocket(i);
		//ajout de notre epoll a sa liste
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock.getSocket(i), &ev) < 0)
		{
			perror("epoll_ctl: sock.getSocket(i)");
			exit(EXIT_FAILURE);
		}

		while (1)
		{
			std::cout << std::endl;
			std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
			std::cout << std::endl;
			//epoll wait for all incoming connection
			if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) < 0)
			{
				perror("epoll_wait()");
				exit(EXIT_FAILURE);
			}
			n = 0;
			std::cout << "nfds = " << nfds << " linstenSock = " << sock.getSocket(i) << " events[n].data.fd = " << events[n].data.fd << std::endl;
			while (n < nfds)
			{
				if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
				{
					/*An error on the fd or the socket is not ready*/
					std::cout << "Epoll error, events = " << events[n].events << std::endl;
					close(events[n].data.fd);
					continue ;
				}
				else if (events[n].data.fd == sock.getSocket(i))
				{
					/*We have one or more incoming connections, notifications on the listening socket*/
					while (1)
					{
						struct sockaddr	in_addr;
						socklen_t		in_addr_len = sizeof(in_addr);

						if ((connSock = accept(sock.getSocket(i), (struct sockaddr *) &in_addr, &in_addr_len)) < 0)
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
						if (setsocknonblock(connSock) < 0)
							exit(EXIT_FAILURE);
						ev.events = EPOLLIN | EPOLLET;
						ev.data.fd = connSock;
						if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock, &ev) < 0)
						{
							perror("epoll_ctl: connSock");
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
				n++;
			}
		}
		close(epollfd);
		close(sock.getSocket(i));
	}
	
	return (0);
}
