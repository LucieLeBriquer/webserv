/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:33:30 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 10:07:53 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"

Socket::Socket()
{
	std::cout << "socket constructor" << std::endl;
}

const int &					Socket::getSocket(int nbr) const
{
	std::vector<int>::const_iterator	it = this->_socket.begin() + nbr;

	return *it;
}

void						Socket::setSocket(int newSocket)
{
	this->_socket.push_back(newSocket);
}

const int &					Socket::getConnSock(int nbr) const
{
	std::vector<int>::const_iterator	it = this->_connSock.begin() + nbr;

	return *it;
}

void						Socket::setConnSock(int newConnSock)
{
	this->_connSock.push_back(newConnSock);
}

int &						Socket::modConnSock(int nbr)
{
	std::vector<int>::iterator	it = this->_connSock.begin() + nbr;

	return *it;
}

const struct sockaddr_in &	Socket::getAddress(int nbr) const
{
	std::vector<struct sockaddr_in>::const_iterator	it = this->_Address.begin() + nbr;

	return *it;
}

void						Socket::setAddress(int port, const char *ip)
{
	struct sockaddr_in	address;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	
	this->_Address.push_back(address);
	this->_addrLen.push_back(sizeof(address));
}

const socklen_t &			Socket::getAddrLen(int nbr) const
{
	std::vector<socklen_t>::const_iterator	it = this->_addrLen.begin() + nbr;

	return *it;
}

int							Socket::getSocketNbr(void) const
{
	return this->_socket.size();
}

Socket::~Socket()
{
	std::cout << "socket destructor" << std::endl;
}

std::ostream &	operator<<(std::ostream &o, Socket const &obj)
{
	for (int i = 0; i < obj.getSocketNbr(); i++)
		o << " listenSock[" << i << "] = " << obj.getSocket(i) << " ";

	return o;
}