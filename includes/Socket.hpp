/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/12 12:49:58 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "Config.hpp"
# include "httpResponse.hpp"
# include "httpRequest.hpp"
# include "httpHeader.hpp"
# include "Client.hpp"
# define MAX_EVENTS 10

class Client;

typedef std::map<int, Client>		mapClient;

class Socket
{
	private:
		vecSrv							_config;
		vecInt							_socket;
		mapSock							_connected;
		mapClient						_clients;
		std::vector<struct sockaddr_in>	_address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		int								_method;
		std::string						_cgiCoprs;
		int								_epollfd;

		int								_initSockets(void);
		void							_setSocket(int newSocket);
		void							_setAddress(int port, const char *ip);

	public:
		Socket();
		Socket(const Socket &socket);
		Socket(const Config &config);
		~Socket();

		Socket	&operator=(const Socket &socket);

		const int &					getSocket(int nbr) const;

		void						addConnection(int connSock, int sockNb);
		int							getConnection(int connSock);
		const mapSock				getAllConnections(void) const;
		
		const struct sockaddr_in &	getAddress(int nbr) const;
		const socklen_t &			getAddrLen(int nbr) const;
		int							getSocketNbr(void) const;
		int							getCheck(void) const;

		int							getEpollFd(void) const;
		void						setEpollFd(int epollfd);
		int							socketMatch(int fde) const;

		mapClient					getClients(void) const;
		Client						&getClient(int fd);
		bool						isConnectedClient(int fd) const;
		void						addClient(int fd);
		void						removeClient(int fd);
	
		int							getMethod(void) const;
		void						setMethod(int method);

		std::string					getCgiCoprs(void) const;
		void						setCgiCoprs(std::string str);

		// config getter
		const Server				getConfig(int nbr) const;
		const Location				getConfig(int nbr, int loc) const;
		int							getConfigFromUrl(int nbr, const std::string url) const;
		std::string					getRealUrl(int nbr, const std::string url) const;
		std::string					errorPage(int nbr, const std::string url, int err) const;
		std::string					getRoot(int nbr, const std::string url) const;
		std::string					addRoot(int nbr, const std::string url, const std::string path) const;
		std::string					getServerName(int nbr) const;
		bool						isAllowedMethod(int nbr, const std::string url, int method) const;
		vecInt						getAllowedMethods(int nbr, const std::string url) const;
		std::string					getHost(int nbr) const;
		std::string					getPort(int nbr) const;
		vecStr						getIndex(int nbr, const std::string url) const;
		bool						hasIndex(int nbr, const std::string url) const;
		bool						getAutoindex(int nbr, const std::string url) const;
		std::string					getRedir(int nbr, const std::string url) const;
		size_t						getMaxClientBodySize(int nbr, const std::string url) const;
		std::string					getCgiPass(int nbr, const std::string url) const;
		bool						isCgi(int nbr, const std::string url) const;
		bool						isRedir(int nbr, const std::string url) const;
		bool						isRootPath(int nbr, const std::string url) const;
		size_t						getNumberListen(void) const;
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				setsocknonblock(int sock);

#endif