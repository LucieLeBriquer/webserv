#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"
# include <map>

class Server
{
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

	public:
		Server(void);
		Server(std::string str);
		Server(const Server &server);
		virtual ~Server();

		Server	&operator=(const Server &server);

		// member functions
		bool	wellFormatted(void) const;
};

std::ostream	&operator<<(std::ostream &o, const Server &server);

#endif
