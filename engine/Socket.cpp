/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:33:30 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/30 10:56:17 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"

Socket::Socket()
{
	std::cout << "socket constructor" << std::endl;
}

int		Socket::getSocket(int nbr)
{
	std::vector<int>::iterator	it;

	it = this->_socket.begin() + nbr;
	return *it;
}

void	Socket::setSocket(int newSocket)
{
	this->_socket.push_back(newSocket);
}

struct sockaddr_in	Socket::getAddress(int nbr)
{
	std::vector<struct sockaddr_in>::iterator	it;

	it = this->_Address.begin() + nbr;
	return *it;
}

void	Socket::setAddress(int port, const char *ip)
{
	struct sockaddr_in	address;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	
	this->_Address.push_back(address);
	this->_addrLen.push_back(sizeof(address));
}

socklen_t	Socket::getAddrLen(int nbr)
{
	std::vector<socklen_t>::iterator	it;

	it = this->_addrLen.begin() + nbr;
	return *it;
}

Socket::~Socket()
{
	std::cout << "socket destructor" << std::endl;
}