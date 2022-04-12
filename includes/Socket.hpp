/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/12 14:35:41 by masboula         ###   ########.fr       */
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
	
		int							getMethod(void) const;
		void						setMethod(int method);

		// config getter
		const Server	getConfig(int nbr) const;
		const Location	getConfig(int nbr, int loc) const;
		int				getConfigFromUrl(int nbr, const std::string url) const;
		std::string		getRealUrl(int nbr, const std::string url) const;
		std::string		errorPage(int nbr, const std::string url, int err) const;
		std::string		getRoot(int nbr, const std::string url) const;
		std::string		getServerName(int nbr) const;
		bool			isAllowedMethod(int nbr, const std::string url, int method) const;
		
	private:
		std::vector<Server>				_config;
		std::vector<int>				_socket;
		std::vector<int>				_connSock;
		std::vector<struct sockaddr_in>	_address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		char**							_env;
		int								_method;
	
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				initSockets(Socket *sock, const Config &config);

#endif