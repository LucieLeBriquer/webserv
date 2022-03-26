/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:56 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 16:19:15 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Server::Server(void) : _host("localhost"), _port(8080), _root("/"), _maxClientBody(0),
					_autoindex(false), _formatOk(true)
{
	return ;
}

Server::Server(const Server &server)
{
	*this = server;
}

Server::Server(std::string str) : _formatOk(true)
{
	std::cout << YELLOW << "[Server] constructor" << END << std::endl;

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
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Server &server)
{
	o << BLUE << "Server" << END << std::endl;
	o << "\t" << "host\t" << server.getHost() << std::endl;
	o << "\t" << "port\t" << server.getPort() << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Server::wellFormatted(void) const
{
	return (_formatOk);
}

void	Server::_setWrontFormat(void)
{
	_formatOk = false;
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
		return (_setWrontFormat());
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

	if (words.size() != 2)
		return (_setWrontFormat());
	splitPattern(addr, words[1], ":");
	if (addr.size() != 2)
		return (_setWrontFormat());
	if (addr[0] == "localhost")
		_host = "127.0.0.1";
	else
	{
		if (!checkHostFormat(addr[0]))
			return (_setWrontFormat());
		_host = addr[0];
	}
	_port = myAtoi(addr[1]);
	if (_port < 0 || _port > 65535)
		return (_setWrontFormat());
}

void	Server::_setServerNames(vecStr words)
{
	return ;
}

void	Server::_setRoot(vecStr words)
{
	return ;
}

void	Server::_setIndex(vecStr words)
{
	return ;
}

void	Server::_setMaxClientBody(vecStr words)
{
	return ;
}

void	Server::_setMethods(vecStr words)
{
	return ;
}

void	Server::_setErrorPages(vecStr words)
{
	return ;
}

void	Server::_setAutoIndex(vecStr words)
{
	return ;
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


std::string Server::keywords[8] = { "listen", "server_name", "root", "index", "client_body",
									"methods", "error_page", "autoindex"};
