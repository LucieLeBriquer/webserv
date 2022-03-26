#include "Server.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

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
	std::cout << "[Server] constructor from :\n" << str << std::endl << std::endl;

	std::vector<std::string> 	locBlocks;
	std::string					serverInfo = "";

	Location::splitBlocks(locBlocks, str.substr(0, str.length() - 2), "location ", serverInfo);
	for (int i = 0; i < locBlocks.size(); i++)
	{
		Location	newLoc(locBlocks[i]);

		if (!newLoc.wellFormatted())
			return ;
		_locations.push_back(newLoc);
	}
	std::cout << "[Other info]" << std::endl << serverInfo << std::endl;
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