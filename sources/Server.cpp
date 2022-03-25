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

Server::Server(std::string fileString, size_t start)
{
	size_t	lBlockPos;

	lBlockPos = fileString.find("location {");
	while (lBlockPos != std::string::npos)
	{
		Location	newLoc(fileString, lBlockPos);
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