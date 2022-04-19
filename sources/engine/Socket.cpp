/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:33:30 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/19 16:34:04 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Socket.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Socket::Socket() : _check(OK), _method(0)
{
	return ;
}

Socket::Socket(const Socket &socket) : _check(OK), _method(0)
{
	*this = socket;
}

Socket::Socket(const Config &config) : _config(config.getServers()), _check(OK), _method(0)
{
	if (initSockets(this, config))
		this->_check = ERR;
}

Socket::~Socket()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

Socket	&Socket::operator=(const Socket &socket)
{
	if (this != &socket)
	{
		_config.clear();
		_socket.clear();
		_connected.clear();
		_address.clear();
		_addrLen.clear();

		_config = socket._config;
		_socket = socket._socket;
		_connected = socket._connected;
		_address = socket._address;
		_addrLen = socket._addrLen;
		_check = socket._check;
		_env = socket._env;
		_method = socket._method;
	}
	return (*this);
}

std::ostream &	operator<<(std::ostream &o, Socket const &obj)
{
	for (int i = 0; i < obj.getSocketNbr(); i++)
		o << " listenSock[" << i << "] = " << obj.getSocket(i) << " ";
	return o;
}

/*
**		MEMBER FUNCTIONS AND SETTERS
*/

void		Socket::setSocket(int newSocket)
{
	_socket.push_back(newSocket);
}

void		Socket::addConnection(int connSock, int sockNb)
{
	_connected[connSock] = sockNb;
}

int			Socket::getConnection(int connSock)
{
	return (_connected[connSock]);
}

const mapSock	Socket::getAllConnections(void) const
{
	return (_connected);
}

void		Socket::setAddress(int port, const char *ip)
{
	struct sockaddr_in	address;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	
	this->_address.push_back(address);
	this->_addrLen.push_back(sizeof(address));
}

void		Socket::setEnv(std::string envp)
{
	this->_env.push_back(envp);
}
void		Socket::setMethod(int method)
{
	this->_method = method;
}

void		Socket::setBody(const char* newBody)
{
	this->_body = newBody;
}
/*
**		GETTER FUNCTIONS
*/

const int &					Socket::getSocket(int nbr) const
{
	return (_socket[nbr]);
}

int							Socket::getMethod(void) const
{
	return this->_method;
}

const Server				Socket::getConfig(int nbr) const
{
	return (_config[nbr]);
}

const Location				Socket::getConfig(int nbr, int loc) const
{
	return (_config[nbr].getLocations()[loc]);
}

std::string	const			Socket::getEnv(int nbr) const
{
	std::vector<std::string>::const_iterator	it = this->_env.begin() + nbr;
	
	return *it;
}

size_t						Socket::getEnvSize(void) const
{
	return this->_env.size();
}

const socklen_t &			Socket::getAddrLen(int nbr) const
{
	return (_addrLen[nbr]);
}

int							Socket::getSocketNbr(void) const
{
	return this->_socket.size();
}

int							Socket::getCheck(void) const
{
	return this->_check;
}

const struct sockaddr_in &	Socket::getAddress(int nbr) const
{
	return (_address[nbr]);
}

const char*						Socket::getBody(void) const
{
	return this->_body;
}

/*
**		URL GETTER FUNCTIONS
*/

int			Socket::getConfigFromUrl(int nbr, const std::string url) const
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
	{
		if (err == 400)
			return ("html/default/400.html");
		if (err == 405)
			return ("html/default/405.html");
		return ("html/default/404.html");
	}
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
	return (removeSlash(root));
}

std::string Socket::addRoot(int nbr, const std::string url, const std::string path) const
{
	std::string	root;

	root = getRoot(nbr, url);
	if (root.size() > 0)
		return (root + "/" + path);
	return (path);
}

std::string Socket::getRealUrl(int nbr, const std::string url) const
{
	return (removeSlash(getConfig(nbr).getRealUrl(url)));
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

std::string	Socket::getHost(int nbr) const
{
	return (getConfig(nbr).getHost());
}

vecStr		Socket::getIndex(int nbr, const std::string url) const
{
	int		nbConfig;
	vecStr	index;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0)
		index = getConfig(nbr).getIndex();
	else
		index = getConfig(nbr, nbConfig).getIndex();
	return (index);
}

bool		Socket::hasIndex(int nbr, const std::string url) const
{
	return (!getIndex(nbr, url).empty());
}

bool		Socket::getAutoindex(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isAutoindexSet())
		return (getConfig(nbr).getAutoIndex());
	return (getConfig(nbr, nbConfig).getAutoIndex());
}

std::string	Socket::getRedir(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isRedirUrlSet())
	{
		if (!getConfig(nbr).isRedirUrlSet())
			return ("");
		return (getConfig(nbr).getRedirUrl());
	}
	return (getConfig(nbr, nbConfig).getRedirUrl());
}

size_t		Socket::getMaxClientBodySize(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isMaxClientBodySet())
		return (getConfig(nbr).getMaxClientBody());
	return (getConfig(nbr, nbConfig).getMaxClientBody());
}

std::string	Socket::getCgiPass(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isCgiPassSet())
		return ("");
	return (getConfig(nbr, nbConfig).getCgiPass());
}

bool		Socket::isRedir(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isRedirUrlSet())
		return (getConfig(nbr).isRedirUrlSet());
	return (true);
}

bool		Socket::isCgi(int nbr, const std::string url) const
{
	int		nbConfig;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isCgiPassSet())
		return (false);
	return (true);
}

bool		Socket::isRootPath(int nbr, const std::string url) const
{
	int			nbConfig;
	Location	loc;
	std::string	path;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0)
		return (url == "/");
	path = url;
	if (url[url.size() - 1] == '/')
		path = url.substr(0, url.size() - 1);
	loc = getConfig(nbr, nbConfig);
	if (loc.getPath() == path)
		return (true);
	return (false);
}
