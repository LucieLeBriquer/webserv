/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:30:07 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/30 11:03:10 by lpascrea         ###   ########.fr       */
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

class Socket
{
	public:

	Socket();
	~Socket();

	int		getSocket(int nbr);
	void	setSocket(int newSocket);
	
	struct sockaddr_in	getAddress(int nbr);
	void				setAddress(int port, const char *ip);

	socklen_t	getAddrLen(int nbr);
	
	private:	

	std::vector<int>				_socket;
	std::vector<struct sockaddr_in>	_Address;
	std::vector<socklen_t>			_addrLen;
	
};

#endif