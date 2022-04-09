/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:33:30 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/08 14:46:03 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Socket.hpp"

Socket::Socket() : _check(0), _methode(0)
{
	return ;
}

Socket::Socket(const Socket &socket) : _check(0), _methode(0)
{
	*this = socket;
}

Socket::Socket(const Config &config) : _config(config.getServers()), _check(0), _methode(0)
{
	if (initSockets(this, config) < 0)
		this->_check = -1;
}

Socket	&Socket::operator=(const Socket &socket)
{
	if (this != &socket)
	{
		_config.clear();
		_socket.clear();
		_connSock.clear();
		_Address.clear();
		_addrLen.clear();

		_config = socket._config;
		_socket = socket._socket;
		_connSock = socket._connSock;
		_Address = socket._Address;
		_addrLen = socket._addrLen;
		_check = socket._check;
	}
	return (*this);
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

int							Socket::getCheck(void) const
{
	return this->_check;
}

Socket::~Socket()
{
	return ;
}

const Server				Socket::getConfig(int nbr) const
{
	return (_config[nbr]);
}

const Location				Socket::getConfig(int nbr, int loc) const
{
	return (_config[nbr].getLocations()[loc]);
}

char**						Socket::getEnv(void) const
{
	return this->_env;
}

void						Socket::setEnv(char** envp)
{
	this->_env = envp;
}

int							Socket::getMethode(void) const
{
	return this->_methode;
}

void						Socket::setMethode(int methode)
{
	this->_methode = methode;
}

std::ostream &	operator<<(std::ostream &o, Socket const &obj)
{
	for (int i = 0; i < obj.getSocketNbr(); i++)
		o << " listenSock[" << i << "] = " << obj.getSocket(i) << " ";

	return o;
}


/*
**		URL GETTER FUNCTIONS
*/

int		Socket::getConfigFromUrl(int nbr, const std::string url) const
{
	return (getConfig(nbr).configFromUrl(url));
}

std::string	Socket::errorPage(int nbr, const std::string url, int err) const
{
	mapErr				pages;
	mapErr::iterator	it;
	int					nbConfig;
	
	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0)
		pages = getConfig(nbr).getErrorPages();
	else
		pages = getConfig(nbr, nbConfig).getErrorPages();
	it = pages.find(err);
	if (it == pages.end())
		return ("/404.html");
	return (getRoot(nbr, url) + "/" + it->second);
}

std::string Socket::getRoot(int nbr, const std::string url) const
{
	std::string	root;
	int			nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	root = getConfig(nbr).getRoot();
	if (nbConfig >= 0 && getConfig(nbr, nbConfig).isRootSet())
		root = getConfig(nbr, nbConfig).getRoot();
	return (root);
}

std::string Socket::getRealUrl(int nbr, const std::string url) const
{
	return (getConfig(nbr).getRealUrl(url));
}

std::string Socket::getServerName(int nbr) const
{
	if (getConfig(nbr).isServerNamesSet())
		return (getConfig(nbr).getServerNames()[0]);
	return ("webserv");
}

bool		Socket::isAllowedMethod(int nbr, const std::string url, int method) const
{
	int			nbConfig;
	vecInt		methods;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isMethodsSet())
	{
		if (!getConfig(nbr).isMethodsSet())
			return (true);
		methods = getConfig(nbr).getMethods();
	}
	else
		methods = getConfig(nbr, nbConfig).getMethods();
	if (find(methods.begin(), methods.end(), method) != methods.end())
		return (true);
	return (false);
}
