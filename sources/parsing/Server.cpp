/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:56 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/06 09:10:43 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Server::Server(void) : _host("localhost"), _port(8080), _hostSet(false), _serverNamesSet(false)
{
	return ;
}

Server::Server(const Server &server) : Block(server)
{
	*this = server;
}

Server::Server(std::string str) :  _host("localhost"), _port(8080), _hostSet(false), _serverNamesSet(false)
{
	std::vector<std::string> 	locBlocks;
	std::string					serverInfo;

	serverInfo.clear();
	if (!splitBlocks(locBlocks, str, "location", serverInfo))
	{
		_formatErr = "matching { } issues in a location block";
		_formatOk = false;
		return ;
	}
	_fillServerInfo(serverInfo);

	for (int i = 0; i < locBlocks.size(); i++)
	{
		Location	newLoc(locBlocks[i]);

		if (!newLoc.wellFormatted())
		{
			_formatOk = false;
			_formatErr = newLoc.getFormatErr();
			return ;
		}
		_locations.push_back(newLoc);
	}
}

Server::~Server()
{
	_locations.clear();
	_serverNames.clear();
}

/*
**		OVERLOAD OPERATORS
*/

Server	&Server::operator=(const Server &server)
{
	if (this != &server)
	{
		(Block)(*this) = (Block)(server);

		_locations.clear();
		_serverNames.clear();

		_locations = server._locations;
		_host = server._host;
		_port = server._port;
		_serverNames = server._serverNames;
		
		_hostSet = server._hostSet;
		_serverNamesSet = server._serverNamesSet;
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
	o << "\tclient_size \t" << server.getMaxClientBody() << std::endl;
	o << "\tmethods     \t";
	showMethod(o, server.getMethods()) << std::endl;
	o << "\terror_pages \t" << server.getErrorPages() << std::endl;
	o << "\treturn      \t" << server.getRedirUrl() << std::endl;
	o << "\tautoindex   \t" << ((server.getAutoIndex() == 0) ? "off" : "on") << std::endl;
	o << server.getLocations() << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

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
	setFunc2	setters[nbKeywords] = {&Server::_setListen, &Server::_setServerNames,
			&Server::_setRoot, &Server::_setIndex, &Server::_setMaxClientBody,
			&Server::_setMethods, &Server::_setErrorPages, &Server::_setAutoIndex,
			&Server::_setRedirUrl};

	splitPattern(words, str, " ");
	keyword = _keywordNumber(words[0]);
	if (keyword < 0)
	{
		_formatOk = false;
		_formatErr = "unknown keyword \"" + words[0] + "\"";
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

std::string	Server::getRealUrl(const std::string &url) const
{
	int			loc;
	std::string	root;
	std::string	path;

	loc = configFromUrl(url);
	if (loc < 0)
		return (_root + "/" + url);
	if (_locations[loc].getPath()[0] == '*')
		return (_root + "/" + url);
	if (_locations[loc].isRootSet())
	{
		root = _locations[loc].getRoot();
		path = _locations[loc].getPath();
		return (root + url.substr(path.size(), url.size() - path.size() + 1));
	}
	return (url);
}

int		Server::configFromUrl(const std::string &url) const
{
	int			maxSize = 0;
	int			matchingLoc = -1;
	int			j;
	std::string	path;
	std::string	extension;
	int			size;

	for (int i = 0; i < _locations.size(); i++)
	{
		j = 0;
		path = _locations[i].getPath();
		size = path.size();
		if (path[0] == '*')
		{
			extension = path.substr(1, path.size() - 2);
			if (url.size() > extension.size()
				&& url.substr(0, url.find('?')).substr(url.size() - extension.size() - 1, extension.size()) == extension)
				return (i);
		}
		else if (url.substr(0, size) == path && ((url.size() > size && url[size] == '/') || url.size() == size)
			&& (size > maxSize))
		{
			maxSize = size;
			matchingLoc = i;
		}
	}
	return (matchingLoc);
}

/*
**		SETTER FUNCTIONS
*/

void	Server::_setListen(vecStr words)
{
	vecStr	addr;
	bool	success;
	size_t	res;

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
	res = myAtoi(addr[1], success);
	if (!success || res > 65535)
		return (_setWrongFormat("wrong PORT"));
	_port = res;
	_hostSet = true;
}

void	Server::_setServerNames(vecStr words)
{
	if (_serverNamesSet)
		return (_setWrongFormat("server_names already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one server name"));
	for (size_t i = 1; i < words.size(); i++)
	{
		if (!checkWordFormat(words[i]))
			return (_setWrongFormat("unvalid SERVER_NAME"));
		_serverNames.push_back(words[i]);
	}
	_serverNamesSet = true;
}

/*
**		GETTER FUNCTIONS
*/

const std::string	Server::getHost(void) const
{
	return (_host);
}

const int			Server::getPort(void) const
{
	return (_port);
}

const vecStr		Server::getServerNames(void) const
{
	return (_serverNames);
}

const vecLoc		Server::getLocations(void) const
{
	return (_locations);
}

const std::string Server::keywords[nbKeywords] = { "listen", "server_name", "root", "index", 
			"client_size", "methods", "error_page", "autoindex", "return"};
