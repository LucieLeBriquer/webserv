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
	size_t	lBlockPos = 0;

	lBlockPos = fileString.find("location {", 0);
	while (lBlockPos != std::string::npos)
	{
		Location	newLoc(fileString, lBlockPos);
		if (!newLoc.wellFormatted())
			return ;
		_locations.push_back(newLoc);
		lBlockPos = fileString.find("location {", lBlockPos + 11); // change 11 with location of last "}"
		std::cout << "new location added " << lBlockPos << std::endl;
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