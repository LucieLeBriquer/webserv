int		main(void)
{
	int					listenSock, connSock, nfds, epollfd, n = 0/*, sockfd*/;
	struct sockaddr_in	address;
	socklen_t			lenAddr;
	char				buf[30000] = {0};
	int					byteCount/*, done*/;
	int					yes = 1;
	struct epoll_event	ev, events[MAX_EVENTS];

	lenAddr = sizeof(address);
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
	address.sin_port = htons(PORT);
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
	//epoll create
	if ((epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		perror("epoll_create()");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listenSock;
	//ajout de notre epoll a sa liste
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenSock, &ev) < 0)
	{
		perror("epoll_ctl: listenSock");
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
		std::cout << "nfds = " << nfds << " linstenSock = " << listenSock << " events[n].data.fd = " << events[n].data.fd << std::endl;
		while (n < nfds)
		{
			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN)))
			{
				/*An error on the fd or the socket is not ready*/
				std::cout << "Epoll error, events = " << events[n].events << std::endl;
				close(events[n].data.fd);
				continue ;
			}
			else if (events[n].data.fd == listenSock)
			{
				/*We have one or more incoming connections, notifications on the listening socket*/
				while (1)
				{
					struct sockaddr	in_addr;
					socklen_t		in_addr_len = sizeof(in_addr);

					if ((connSock = accept(listenSock, (struct sockaddr *) &in_addr, &in_addr_len)) < 0)
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
	close(listenSock);
	return 0;
}