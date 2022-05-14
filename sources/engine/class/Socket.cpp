/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:33:30 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/12 16:48:45 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Socket::Socket() : _check(OK), _method(0), _epollfd(-1)
{
	return ;
}

Socket::Socket(const Socket &socket) : _check(OK), _method(0), _epollfd(-1)
{
	*this = socket;
}

Socket::Socket(const Config &config) : _config(config.getServers()), _check(OK), _method(0), _epollfd(-1)
{
	if (_initSockets())
		this->_check = ERR;
}

Socket::~Socket()
{
	_config.clear();
	_socket.clear();
	_connected.clear();
	_address.clear();
	_addrLen.clear();
	_clients.clear();
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
		_clients.clear();

		_config = socket._config;
		_socket = socket._socket;
		_connected = socket._connected;
		_clients = socket._clients;
		_address = socket._address;
		_addrLen = socket._addrLen;
		_check = socket._check;
		_method = socket._method;
		_epollfd = socket._epollfd;
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

int 	Socket::socketMatch(int fde) const
{
	for (int i = 0; i < getSocketNbr(); i++)
	{
		if (fde == getSocket(i))
			return (i);
	}
	return (ERR);
}

int		Socket::_initSockets(void)
{
	int			listenSock;
	int			yes = 1;
	vecSrv		servers = _config;

	for (size_t i = 0; i < servers.size(); i++)
	{
		if ((listenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket()");
			return (ERR);
		}
		_setSocket(listenSock);
		if (setsockopt(getSocket(i), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
		{
			perror("setsockopt()");
			return (ERR);
		}
		_setAddress(servers[i].getPort(), servers[i].getHost().c_str());
		if (bind(getSocket(i), (struct sockaddr *)&getAddress(i), (int)getAddrLen(i)) < 0)
		{
			perror("bind()");
			return (ERR);
		}
		if (setsocknonblock(getSocket(i)))
			return (ERR);
		if (listen(getSocket(i), MAX_EVENTS) < 0)
		{
			perror("listen()");
			return (ERR);
		}
	}
	return (OK);
}

void		Socket::_setSocket(int newSocket)
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

void		Socket::_setAddress(int port, const char *ip)
{
	struct sockaddr_in	address;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	
	this->_address.push_back(address);
	this->_addrLen.push_back(sizeof(address));
}

void		Socket::setMethod(int method)
{
	this->_method = method;
}

void		Socket::setEpollFd(int epollfd)
{
	_epollfd = epollfd;
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

size_t						Socket::getNumberListen(void) const
{
	return (_config.size());
}

int							Socket::getEpollFd(void) const
{
	return (_epollfd);
}

/*
**		URL GETTER FUNCTIONS
*/

int			Socket::getConfigFromUrl(int nbr, const std::string url) const
{
	return (getConfig(nbr).configFromUrl(url));
}

static std::string	getRelativePath(const std::string url)
{
	vecStr		splitted;
	std::string	relative = "";
	
	splitPattern(splitted, url, "/");
	for (size_t i = 0; i + 2 < splitted.size(); i++)
		relative += "../";
	return (relative);
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
		return ("");
	return (getRelativePath(url) + it->second);
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
	return ("");
}

bool		Socket::isAllowedMethod(int nbr, const std::string url, int method) const
{
	int			nbConfig;
	vecInt		methods;

	nbConfig = getConfigFromUrl(nbr, url);
	if (nbConfig < 0 || !getConfig(nbr, nbConfig).isMethodsSet())
	{
		if (!getConfig(nbr).isMethodsSet())
			return (method == getMethodNb("GET") || method == getMethodNb("HEAD") || method == getMethodNb("OPTIONS"));
		methods = getConfig(nbr).getMethods();
	}
	else
		methods = getConfig(nbr, nbConfig).getMethods();
	if (find(methods.begin(), methods.end(), method) != methods.end())
		return (true);
	return (false);
}

vecInt		Socket::getAllowedMethods(int nbr, const std::string url) const
{
	vecInt	methods;

	for (int i = 0; i < nbMethods; i++)
	{
		if (isAllowedMethod(nbr, url, i))
			methods.push_back(i);
	}
	return (methods);
}

std::string	Socket::getHost(int nbr) const
{
	return (getConfig(nbr).getHost());
}

std::string	Socket::getPort(int nbr) const
{
	return (toString(getConfig(nbr).getPort()));
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

/*	
**		CLIENT FUNCTIONS
*/

mapClient	Socket::getClients(void) const
{
	return (_clients);
}

Client		&Socket::getClient(int fd)
{
	return ((_clients.find(fd))->second);
}

bool		Socket::isConnectedClient(int fd) const
{
  	if (_clients.find(fd) != _clients.end())
	  	return (true);
	return (false);
}

void		Socket::addClient(int fd)
{
	if (!isConnectedClient(fd))
		_clients[fd] = Client(fd);
}

void		Socket::removeClient(int fd)
{
	if (isConnectedClient(fd))
		_clients.erase(fd);
}

/*	
**		USEFULL
*/

int		setsocknonblock(int sock)
{
	int flag;

	flag = fcntl(sock, F_GETFL, 0);
	if (flag < 0)
	{
		perror("Fcntl (F_GETFL) failed");
		return (ERR);
	}
	if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		perror("Fcntl (F_SETFL) failed");
		return (ERR);
	}
	return (OK);
}
