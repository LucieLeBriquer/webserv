#include "Server.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Server::Server(void) : _formatOk(true)
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
	Location::splitBlocks(locBlocks, str, "location", serverInfo);
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
		/* code */
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Server &server)
{
	(void)server;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Server::wellFormatted(void) const
{
	return (_formatOk);
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

	Location::splitPattern(words, str, " ");
	keyword = _keywordNumber(words[0]);
	if (keyword < 0)
	{
		_formatOk = false;
		return ;
	}
	
}

void	Server::_fillServerInfo(std::string str)
{
	std::vector<std::string>	lines;

	Location::splitPattern(lines, str, "\n");
	if (!isBlockNameOk(lines[0], "server"))
	{
		_formatOk = false;
		return ;
	}
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
	return ;
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


std::string Server::keywords[8] = { "listen", "server_name", "root", "index", "client_body",
									"methods", "error_page", "autoindex"};
