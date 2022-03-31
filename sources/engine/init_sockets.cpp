/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:27:43 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 11:18:47 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/engine.hpp"

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

int		init_sockets(Socket *sock, std::map<int, const char *> map)
{
	int					listenSock;
	int					yes = 1;

	for (int i = 0; i < (int)map.size(); i++)
	{
		if ((listenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket()");
			return -1;
		}
		sock->setSocket(listenSock);
		sock->setConnSock(0);
		if (setsockopt(sock->getSocket(i), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
		{
			perror("setsockopt()");
			return -1;
		}
		std::map<int, const char *>::iterator	it = map.begin();
		for (int j = 0; j < i; j++)
			++it;
		sock->setAddress(it->first, it->second);
		if (bind(sock->getSocket(i), (struct sockaddr *)&sock->getAddress(i), (int)sock->getAddrLen(i)) < 0)
		{
			perror("bind()");
			return -1;
		}
		if (setsocknonblock(sock->getSocket(i)) < 0)
			return -1;
		if (listen(sock->getSocket(i), MAX_EVENTS) < 0)
		{
			perror("listen()");
			return -1;
		}
	}
	return 1;
}

int		initSockets(Socket *sock, const Config config)
{
	int			listenSock;
	int			yes = 1;
	vecSrv		servers = config.getServers();

	for (int i = 0; i < servers.size(); i++)
	{
		if ((listenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket()");
			return -1;
		}
		sock->setSocket(listenSock);
		sock->setConnSock(0);
		if (setsockopt(sock->getSocket(i), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
		{
			perror("setsockopt()");
			return -1;
		}
		sock->setAddress(servers[i].getPort(), servers[i].getHost().c_str());
		if (bind(sock->getSocket(i), (struct sockaddr *)&sock->getAddress(i), (int)sock->getAddrLen(i)) < 0)
		{
			perror("bind()");
			return -1;
		}
		if (setsocknonblock(sock->getSocket(i)) < 0)
			return -1;
		if (listen(sock->getSocket(i), MAX_EVENTS) < 0)
		{
			perror("listen()");
			return -1;
		}
	}
	return 1;
}
