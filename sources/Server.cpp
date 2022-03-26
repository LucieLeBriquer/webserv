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
	Location::splitBlocks(locBlocks, str, "location ", serverInfo);
	//std::cout << BLUE << "serverInfo" << END << "\n--->" << serverInfo << "<---" << std::endl;
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

void	Server::_fillOneInfo(std::string str)
{
	std::cout << str << std::endl;
}

void	Server::_fillServerInfo(std::string str)
{
	std::vector<std::string>	lines;

	Location::splitLines(lines, str);
	if (!isBlockNameOk(lines[0], "server"))
	{
		_formatOk = false;
		return ;
	}
	for (int i = 1; i + 1 < lines.size(); i++)
	{
		_fillOneInfo(lines[i]);
	}
}


/*

private:
	std::vector<Location>		_locations;
	std::string					_host;
	int							_port;
	std::string					_root;
	std::string					_index;
	size_t						_maxClientBody;
	std::vector<int>			_methods;
	std::map<int, std::string>	_errorPages;
	bool						_autoindex;
	bool						_formatOk;

*/

std::string Server::keywords[8] = { "listen", "server_name", "root", "index", "client_body",
									"methods", "error_page", "autoindex"};