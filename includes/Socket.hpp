/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/08 14:44:41 by lpascrea         ###   ########.fr       */
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
		Socket(const Socket &socket);
		Socket(const Config &config);
		~Socket();

		Socket	&operator=(const Socket &socket);

		const int &					getSocket(int nbr) const;
		void						setSocket(int newSocket);

		const Server				getConfig(int nbr) const;
		const Location				getConfig(int nbr, int loc) const;

		const int &					getConnSock(int nbr) const;
		void						setConnSock(int newConnSock);
		int &						modConnSock(int nbr);
		
		const struct sockaddr_in &	getAddress(int nbr) const;
		void						setAddress(int port, const char *ip);

		const socklen_t &			getAddrLen(int nbr) const;

		int							getSocketNbr(void) const;
		int							getCheck(void) const;

		char**						getEnv(void) const;
		void						setEnv(char** envp);
	
		int							getMethode(void) const;
		void						setMethode(int methode);
		
	private:
		std::vector<Server>				_config;
		std::vector<int>				_socket;
		std::vector<int>				_connSock;
		std::vector<struct sockaddr_in>	_Address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		char**							_env;
		int								_methode;
	
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				initSockets(Socket *sock, const Config &config);

#endif