/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:56 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 15:09:37 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

/*
vecLoc		_locations;
vecStr		_serverNames;
vecStr		_index;
vecInt		_methods;
mapErr		_errorPages;
*/

Server::Server(void) : _host("localhost"), _port(8080), _root("/"), _maxClientBody(0),
					_autoindex(false), _formatOk(true), _formatErr(""), _hostSet(false), 
					_serverNamesSet(false), _rootSet(false), _indexSet(false), _maxClientBodySet(false), 
					_methodsSet(false), _errorPagesSet(false), _autoindexSet(false)
{
	return ;
}

Server::Server(const Server &server)
{
	*this = server;
}

Server::Server(std::string str) :  _host("localhost"), _port(8080), _root("/"), _maxClientBody(0),
					_autoindex(false), _formatOk(true), _formatErr(""), _hostSet(false), 
					_serverNamesSet(false), _rootSet(false), _indexSet(false), _maxClientBodySet(false), 
					_methodsSet(false), _errorPagesSet(false), _autoindexSet(false)
{
	std::vector<std::string> 	locBlocks;
	std::string					serverInfo;

	serverInfo.clear();
	splitBlocks(locBlocks, str, "location", serverInfo);
	_fillServerInfo(serverInfo);

	for (int i = 0; i < locBlocks.size(); i++)
	{
		Location	newLoc(locBlocks[i]);

		if (!newLoc.wellFormatted())
			return ;
		_locations.push_back(newLoc);
	}
}

Server::~Server()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

Server	&Server::operator=(const Server &server)
{
	if (this != &server)
	{
		_locations = server._locations;
		_host = server._host;
		_port = server._port;
		_serverNames = server._serverNames;
		_root = server._root;
		_index = server._index;
		_maxClientBody = server._maxClientBody;
		_methods = server._methods;
		_errorPages = server._errorPages;
		_autoindex = server._autoindex;
		_formatOk = server._formatOk;
		_formatErr = server._formatErr;
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Server &server)
{
	size_t	maxCl;
	
	o << BLUE << "Server" << END << std::endl;
	o << "\thost        \t" << server.getHost() << std::endl;
	o << "\tport        \t" << server.getPort() << std::endl;
	o << "\tserver_names\t" << server.getServerNames() << std::endl;
	o << "\troot        \t" << server.getRoot() << std::endl;
	o << "\tindex       \t" << server.getIndex() << std::endl;
	o << "\tclient_body \t" << server.getMaxClientBody() << std::endl;
	o << "\tmethods     \t";
	showMethod(o, server.getMethods()) << std::endl;
	o << "\terror_pages \t" << server.getErrorPages() << std::endl;
	o << "\tautoindex   \t" << ((server.getAutoIndex() == 0) ? "off" : "on") << std::endl;
	o << server.getLocations() << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Server::wellFormatted(void) const
{
	return (_formatOk);
}

void	Server::_setWrongFormat(void)
{
	_formatOk = false;
}


void	Server::_setWrongFormat(std::string str)
{
	_formatOk = false;
	_formatErr = str;
}

static bool	isBlockNameOk(std::string str, std::string pattern)
{
	size_t	i = pattern.size();

	while (i < str.length() - 1)
	{
		if (!isspace(str.c_str()[i]))
			return (false);
		i++;
	}
	return (true);
}

int		Server::_keywordNumber(std::string str)
{
	for (int i = 0; i < nbKeywords; i++)
	{
		if (str == keywords[i])
			return (i);
	}
	_formatOk = false;
	return (-1);
}

void	Server::_fillOneInfo(std::string str)
{
	int		keyword;
	vecStr	words;
	setFunc	setters[nbKeywords] = {&Server::_setListen, &Server::_setServerNames,
			&Server::_setRoot, &Server::_setIndex, &Server::_setMaxClientBody,
			&Server::_setMethods, &Server::_setErrorPages, &Server::_setAutoIndex};

	splitPattern(words, str, " ");
	keyword = _keywordNumber(words[0]);
	if (keyword < 0)
	{
		_formatOk = false;
		return ;
	}
	return ((this->*(setters[keyword]))(words));
}

void	Server::_fillServerInfo(std::string str)
{
	vecStr	lines;

	splitPattern(lines, str, "\n");
	if (!isBlockNameOk(lines[0], "server"))
		return (_setWrongFormat("wrong start of server block"));
	for (int i = 1; i + 1 < lines.size(); i++)
	{
		_fillOneInfo(lines[i]);
		if (!_formatOk)
			return ;
	}
}

/*
**		SETTER FUNCTIONS
*/

void	Server::_setListen(vecStr words)
{
	vecStr	addr;

	if (_hostSet)
		return (_setWrongFormat("host and port already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need listen HOST:PORT"));
	splitPattern(addr, words[1], ":");
	if (addr.size() != 2)
		return (_setWrongFormat("need listen HOST:PORT"));
	if (addr[0] == "localhost")
		_host = "127.0.0.1";
	else
	{
		if (!checkHostFormat(addr[0]))
			return (_setWrongFormat("wrong HOST format"));
		_host = addr[0];
	}
	_port = myAtoi(addr[1]);
	if (_port < 0 || _port > 65535)
		return (_setWrongFormat("wrong PORT"));
	_hostSet = true;
}

void	Server::_setServerNames(vecStr words)
{
	if (_serverNamesSet)
		return (_setWrongFormat("server_names already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one server name"));
	for (size_t i = 1; i < words.size(); i++)
		_serverNames.push_back(words[i]);
	_serverNamesSet = true;
}

void	Server::_setRoot(vecStr words)
{
	if (_rootSet)
		return (_setWrongFormat("root already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need root"));
	_root = words[1];
	_rootSet = true;
}

void	Server::_setIndex(vecStr words)
{
	if (_indexSet)
		return (_setWrongFormat("index already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one index page"));
	for (size_t i = 1; i < words.size(); i++)
		_index.push_back(words[i]);
	_indexSet = true;
}

void	Server::_setMaxClientBody(vecStr words)
{
	if (_maxClientBodySet)
		return (_setWrongFormat("client_body already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need client_body SIZE"));
	_maxClientBody = myAtoi(words[1]);
	if (_maxClientBody < 0)
		return (_setWrongFormat("size for client_body not well formatted"));
	if (_maxClientBody == 0)
		_maxClientBody = std::numeric_limits<size_t>::max();
	_maxClientBodySet = true;
}

void	Server::_setMethods(vecStr words)
{
	if (_methodsSet)
		return (_setWrongFormat("methods already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one method"));
	for (size_t i = 1; i < words.size(); i++)
	{
		int method = getMethodNb(words[i]);
		if (method < 0)
			return (_setWrongFormat(words[i] + " is an unkown method"));
		_methods.push_back(method);
	}
	_methodsSet = true;
}

void	Server::_setErrorPages(vecStr words) // TODO
{
	std::pair<int, std::string>	err;
	
	if (words.size() != 3)
		return (_setWrongFormat("need error ERROR_NUM ERROR_PAGE"));
	err.first = myAtoi(words[1]);
	if (err.first < 0)
		return (_setWrongFormat("unvalid ERROR_NUM"));
	err.second = words[2];
	_errorPages.insert(err);
	_errorPagesSet = true;
}

void	Server::_setAutoIndex(vecStr words)
{
	if (_autoindexSet)
		return (_setWrongFormat("error_pages already defined"));
	if (words.size() != 1)
		return (_setWrongFormat("autoindex doesn't take arguments"));
	_autoindex = true;
	_autoindexSet = true;
}

/*
**		GETTER FUNCTIONS
*/

std::string	Server::getHost(void) const
{
	return (_host);
}

int			Server::getPort(void) const
{
	return (_port);
}

vecStr		Server::getServerNames(void) const
{
	return (_serverNames);
}

std::string	Server::getRoot(void) const
{
	return (_root);
}

vecStr		Server::getIndex(void) const
{
	return (_index);
}

size_t		Server::getMaxClientBody(void) const
{
	return (_maxClientBody);
}

vecInt		Server::getMethods(void) const
{
	return (_methods);
}

mapErr		Server::getErrorPages(void) const
{
	return (_errorPages);
}

bool		Server::getAutoIndex(void) const
{
	return (_autoindex);
}

vecLoc		Server::getLocations(void) const
{
	return (_locations);
}

std::string	Server::getFormatErr(void) const
{
	return (_formatErr);
}


std::string Server::keywords[8] = { "listen", "server_name", "root", "index", "client_body", "methods", "error_page", "autoindex"};
