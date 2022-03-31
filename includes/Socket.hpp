/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 09:26:06 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstring>
# include "Config.hpp"

class Socket
{
	public:
		Socket();
		Socket(const Config config);
		~Socket();

		const int &					getSocket(int nbr) const;
		void						setSocket(int newSocket);

		const int &					getConnSock(int nbr) const;
		void						setConnSock(int newConnSock);
		int &						modConnSock(int nbr);
		
		const struct sockaddr_in &	getAddress(int nbr) const;
		void						setAddress(int port, const char *ip);

		const socklen_t &			getAddrLen(int nbr) const;

		int							getSocketNbr(void) const;
	
	private:
		std::vector<Server>				_config;
		std::vector<int>				_socket;
		std::vector<int>				_connSock;
		std::vector<struct sockaddr_in>	_Address;
		std::vector<socklen_t>			_addrLen;
	
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);

#endif