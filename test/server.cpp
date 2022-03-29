#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE	10

int		setsocknonblock(int sock)
{
	int flag;

	flag = fcntl(sock, F_GETFL, 0);
	if (flag < 0)
	{
		perror("Fcntl (F_GETFL) failed");
		return -1;
	}
	if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		perror("Fcntl (F_SETFL) failed");
		return -1;
	}
	return 1;
}

void	socketCreation(int &listenSock, struct sockaddr_in &address, int lenAddr, int port)
{
	int yes = 1;
	//create socket
	if ((listenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	//ajout de flag sur notre socket
	if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	//bind le socket
	if (bind(listenSock, (struct sockaddr *)&address, (int)lenAddr) < 0)
	{
		perror("bind()");
		exit(EXIT_FAILURE);
	}

/*	if ((sockfd = getsockname(listenSock, (struct sockaddr *)&address, &lenAddr)) < 0)
	{
		perror("getsockname()");
		exit(EXIT_FAILURE);
	}
	std::cout << "sockfd (getsockname) = " << sockfd << std::endl;*/

	//socket en mode non blocking
	if (setsocknonblock(listenSock) < 0)
		exit(EXIT_FAILURE);
		
	//listen the socket
	if (listen(listenSock, MAX_EVENTS) < 0)
	{
		perror("listen()");
		exit(EXIT_FAILURE);
	}
}

int		main(void)
{
	int					nfds, epollfd, n = 0/*, sockfd*/;

	int					listenSock1;
	int					listenSock2;
	struct sockaddr_in	address1;
	struct sockaddr_in	address2;

	int					connSock1;
	int					connSock2;

	socklen_t			lenAddr;
	char				buf[30000] = {0};
	int					byteCount/*, done*/;
	struct epoll_event	ev, events[MAX_EVENTS];

	lenAddr = sizeof(address1);
	
	socketCreation(listenSock1, address1, lenAddr, 8080);
	socketCreation(listenSock2, address2, lenAddr, 8081);


	//epoll create
	if ((epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		perror("epoll_create()");
		exit(EXIT_FAILURE);
	}


	//ajout de notre socket2 a sa liste
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listenSock1;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSock1, &ev) < 0)
	{
		perror("epoll_ctl: listenSock1");
		exit(EXIT_FAILURE);
	}

	//ajout de notre socket2 a sa liste
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listenSock2;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSock2, &ev) < 0)
	{
		perror("epoll_ctl: listenSock2");
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
		std::cout << "nfds : " << nfds << std::endl;
		std::cout << "\tlistenSock1 " << listenSock1 << std::endl;
		std::cout << "\tlistenSock2 " << listenSock2 << std::endl;
		std::cout << "\tevent[n].data.fd " << events[n].data.fd << std::endl;
		while (n < nfds)
		{

			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
			{
				/*An error on the fd or the socket is not ready*/
				std::cout << "Epoll error, events = " << events[n].events << std::endl;
				close(events[n].data.fd);
				continue ;
			}
			else if (events[n].data.fd == listenSock1)
			{
				std::cout << "connecting to socket " << listenSock1 << std::endl;
				/*We have one or more incoming connections, notifications on the listening socket*/
				while (1)
				{
					struct sockaddr	in_addr;
					socklen_t		in_addr_len = sizeof(in_addr);

					if ((connSock1 = accept(listenSock1, (struct sockaddr *) &in_addr, &in_addr_len)) < 0)
					{
					//	std::cout << "errno = " << errno << " EAGAIN = " << EAGAIN << " EWOULDBLOCK = " << EWOULDBLOCK << std::endl;
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
					if (setsocknonblock(connSock1) < 0)
						exit(EXIT_FAILURE);

					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = connSock1;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock1, &ev) < 0)
					{
						perror("epoll_ctl: connSock");
						exit(EXIT_FAILURE);
					}
				}
			}
			else if (events[n].data.fd == listenSock2)
			{
				std::cout << "connecting to socket " << listenSock2 << std::endl;
				/*We have one or more incoming connections, notifications on the listening socket*/
				while (1)
				{
					struct sockaddr	in_addr;
					socklen_t		in_addr_len = sizeof(in_addr);

					if ((connSock2 = accept(listenSock2, (struct sockaddr *) &in_addr, &in_addr_len)) < 0)
					{
					//	std::cout << "errno = " << errno << " EAGAIN = " << EAGAIN << " EWOULDBLOCK = " << EWOULDBLOCK << std::endl;
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
					if (setsocknonblock(connSock2) < 0)
						exit(EXIT_FAILURE);

					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = connSock2;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock2, &ev) < 0)
					{
						perror("epoll_ctl: connSock");
						exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
				/*Data waiting to be read on our waiting fd*/
			//	done = 0;
				memset(buf, 0, sizeof(buf));
				while (1)
				{
					byteCount = recv(events[n].data.fd, buf, sizeof(buf), /*MSG_DONTWAIT*/0);
					if (byteCount <= 0)
					{
			//			done = 1;
						if (byteCount == 0)
						{
							std::cout << "Finished with " << events[n].data.fd << std::endl;
							close(events[n].data.fd);
						}
						break ;
					}
					else
					{
						std::cout << "get " << byteCount << " bytes of content from " << events[n].data.fd << " [ " << buf << " ] " << std::endl;
						if (send(events[n].data.fd, buf, byteCount, /*MSG_DONTWAIT*/0) < 0)
						{
							perror("send()");
							exit(EXIT_FAILURE);
						}
						std::cout << "sending data to " << events[n].data.fd << std::endl;
					}
				}
			//	if (done)
			//		close(events[n].data.fd);
			}
			n++;
		}
	}
	close(epollfd);
	close(listenSock1);
	close(listenSock2);
	return 0;
}
