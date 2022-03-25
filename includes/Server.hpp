#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"

class Server
{
	private:
		std::vector<Location>	_locations;
		bool					_formatOk;
		
	public:
		Server(void);
		Server(std::string fileString, size_t start);
		Server(const Server &server);
		virtual ~Server();

		Server	&operator=(const Server &server);

		// member functions
		bool	wellFormatted(void) const;
};

std::ostream	&operator<<(std::ostream &o, const Server &server);

#endif
