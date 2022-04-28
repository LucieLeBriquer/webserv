/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/27 14:46:55 by user42           ###   ########.fr       */
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

		void						addConnection(int connSock, int sockNb);
		int							getConnection(int connSock);
		const mapSock				getAllConnections(void) const;
		
		const struct sockaddr_in &	getAddress(int nbr) const;
		void						setAddress(int port, const char *ip);

		const socklen_t &			getAddrLen(int nbr) const;

		int							getSocketNbr(void) const;
		int							getCheck(void) const;

		mapStr						getEnv(void ) const;
		std::string					getEnvValue(std::string envp);
		void						setEnv(std::string envp);
		void						setEnvValue(std::string envp, std::string value);
		size_t						getEnvSize(void) const;
		void						unsetEnv(void);
	
		bool						isQueryString(void) const;
		void						setIsQueryString(bool set);
	
		int							getMethod(void) const;
		void						setMethod(int method);

		std::string					getBody(void) const;
		void						setBody(std::string newBody);

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
		
	private:
		vecSrv							_config;
		vecInt							_socket;
		mapSock							_connected;
		std::vector<struct sockaddr_in>	_address;
		std::vector<socklen_t>			_addrLen;
		int								_check;
		mapStr							_env;
		int								_method;
		std::string						_body;
		bool							_isQuery;
		std::string						_cgiCoprs;
};

std::ostream &	operator<<(std::ostream &o, Socket const &obj);
int				initSockets(Socket *sock, const Config &config);

#endif