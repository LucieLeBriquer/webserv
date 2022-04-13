/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/13 13:53:31 by lle-briq         ###   ########.fr       */
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

		std::string	const			getEnv(int nbr) const;
		void						setEnv(std::string envp);
		size_t						getEnvSize(void) const;
	
		int							getMethod(void) const;
		void						setMethod(int method);

		char*						getBody(void) const;
		void						setBody(char* newBody);

		// config getter
		const Server	getConfig(int nbr) const;
		const Location	getConfig(int nbr, int loc) const;
		int				getConfigFromUrl(int nbr, const std::string url) const;
		std::string		getRealUrl(int nbr, const std::string url) const;
		std::string		errorPage(int nbr, const std::string url, int err) const;
		std::string		getRoot(int nbr, const std::string url) const;
		std::string		getServerName(int nbr) const;
		bool			isAllowedMethod(int nbr, const std::string url, int method) const;
		std::string		getHost(int nbr) const;
		vecStr			getIndex(int nbr, const std::string url) const;
		bool			hasIndex(int nbr, const std::string url) const;
		bool			getAutoindex(int nbr, const std::string url) const;
		std::string		getRedir(int nbr, const std::string url) const;
		size_t			getMaxClientBodySize(int nbr, const std::string url) const;
		std::string		getCgiPass(int nbr, const std::string url) const;
		bool			isCgi(int nbr, const std::string url) const;
		bool			isRedir(int nbr, const std::string url) const;
		
	private:
		std::vector<Server>				_config;
		std::vector<int>				_socket;
		std::vector<int>				_connSock;
		std::vector<struct sockaddr_in>	_address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		std::vector<std::string>		_env;
		int								_method;
		char*							_body;
	
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				initSockets(Socket *sock, const Config &config);

#endif