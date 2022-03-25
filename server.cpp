#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10

int		main(void)
{
	int					listenSock, connSock, nfds, epollfd, n = 0;
	struct sockaddr_in	address;
	socklen_t			lenAddr;
	char				buf[1024] = {0};
	int					byteCount;
	int					yes = 1;
	struct epoll_event	ev, events[MAX_EVENTS];

	lenAddr = sizeof(address);
//TCP socket
	if (listenSock = socket(AF_INET, SOCK_STREAM, 0) < 0)
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}
// Forcefully attaching socket to the port 8080
	if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.2");
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if (bind(listenSock, (struct sockaddr *)&address, (int)lenAddr) < 0)
	{
		perror("bind()");
		exit(EXIT_FAILURE);
	}
	if (listen(listenSock, MAX_EVENTS) < 0)
	{
		perror("listen()");
		exit(EXIT_FAILURE);
	}

//epoll
	if ((epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		perror("epoll_create()");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listenSock;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSock, &ev) < 0)
	{
		perror("epoll_ctl: listenSock");
		exit(EXIT_FAILURE);
	}

	for (;;)
	{
		if (nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1) < 0)
		{
			perror("epoll_wait()");
			exit(EXIT_FAILURE);
		}
		n = 0;
		while (n < nfds)
		{
			if (events[n].data.fd == listenSock)
			{
				if ((connSock = accept(listenSock, (struct sockaddr *) &address, &lenAddr)) < 0)
				{
					perror("accept()");
					exit(EXIT_FAILURE);
				}
				//set non blocking connSock
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connSock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connSock, &ev) < 0)
				{
					perror("epoll_ctl: connSock");
					exit(EXIT_FAILURE);
				}
			}
			else
				//use fd
			n++;
		}
	}
	close(epollfd);

/*	while (1)
	{
		std::cout << std::endl;
		std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
		std::cout << std::endl;
		if ((connSock = accept(listenSock, (struct sockaddr *)&address, &lenAddr)) < 0)
		{
			perror("accept()");
			exit(EXIT_FAILURE);
		}
		byteCount = recv(connSock, buf, sizeof(buf), 0);
		std::cout << buf << std::endl;
		send(connSock, "Hello world!", 12, 0);
		std::cout << "---------- Hello message sent ----------" << std::endl;
		close(new_socket);
	}*/
	return 0;
}
