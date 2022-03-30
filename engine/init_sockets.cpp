/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:27:43 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/30 10:58:17 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/engine.hpp"

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

int		init_sockets(Socket *sock, std::map<int, char> map)
{
	int					listenSock;
	struct sockaddr_in	address;
	socklen_t			lenAddr = sizeof(address);
	int					yes = 1;

	for (int i = 0; i < map.size(); i++)
	{
		if ((listenSock = socket(AF8INET, SOCK_STREAM, 0) < 0)
		{
			perror("socket()");
			return -1;
		}
		sock->setSocket(listenSock);
		if (setsockopt(sock->getSocket(i), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
		{
			perror("setsockopt()");
			return -1;
		}
		sock->setAddress(map.first, map.second);
		if (bind(sock->getSocket(i), (struct sockaddr *)&sock->getAddress(), (int)sock->getAddrLen()) < 0)
		{
			perror("bind()");
			return -1;
		}
		// if ((sockfd = getsockname(listenSock, (struct sockaddr *)&address, &lenAddr)) < 0)
		// {
		// 	perror("getsockname()");
		// 	exit(EXIT_FAILURE);
		// }
		// std::cout << "sockfd (getsockname) = " << sockfd << std::endl;
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