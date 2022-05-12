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
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstring>
# include "Config.hpp"
# define MAX_EVENTS 10

class Socket
{
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
		
		mapStr						getEnv(void) const;
		std::string					getEnvValue(std::string envp);
		void						setEnv(std::string envp);
		void						setEnvValue(std::string envp, std::string value);
		size_t						getEnvSize(void) const;
		void						unsetEnv(void);
	
		bool						isQueryString(void) const;
		void						setIsQueryString(bool set);
	
		int							getMethod(void) const;
		void						setMethod(int method);

		FILE *						getBody(void) const;
		void						setBody(FILE *newBody);
		void						unsetBody(FILE *oldBody);

		int							getFdBody(void) const;
		void						setFdBody(int newFdBody);
		void						unsetFdBody(int oldFdBody);

		std::string					getCgiCoprs(void) const;
		void						setCgiCoprs(std::string str);

		// config getter
		const Server	getConfig(int nbr) const;
		const Location	getConfig(int nbr, int loc) const;
		int				getConfigFromUrl(int nbr, const std::string url) const;
		std::string		getRealUrl(int nbr, const std::string url) const;
		std::string		errorPage(int nbr, const std::string url, int err) const;
		std::string		getRoot(int nbr, const std::string url) const;
		std::string		addRoot(int nbr, const std::string url, const std::string path) const;
		std::string		getServerName(int nbr) const;
		bool			isAllowedMethod(int nbr, const std::string url, int method) const;
		vecInt			getAllowedMethods(int nbr, const std::string url) const;
		std::string		getHost(int nbr) const;
		vecStr			getIndex(int nbr, const std::string url) const;
		bool			hasIndex(int nbr, const std::string url) const;
		bool			getAutoindex(int nbr, const std::string url) const;
		std::string		getRedir(int nbr, const std::string url) const;
		size_t			getMaxClientBodySize(int nbr, const std::string url) const;
		std::string		getCgiPass(int nbr, const std::string url) const;
		bool			isCgi(int nbr, const std::string url) const;
		bool			isRedir(int nbr, const std::string url) const;
		bool			isRootPath(int nbr, const std::string url) const;
		size_t			getNumberListen(void) const;
		
	private:
		vecSrv							_config;
		vecInt							_socket;
		mapSock							_connected;
		std::vector<struct sockaddr_in>	_address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		mapStr							_env;
		int								_method;
		bool							_isQuery;
		std::string						_cgiCoprs;
		FILE*							_body;
		int								_fdBody;
		int								_epollfd;

		int								_initSockets(void);
		void							_setSocket(int newSocket);
		void							_setAddress(int port, const char *ip);
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				setsocknonblock(int sock);

#endif