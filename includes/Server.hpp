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
		std::vector<std::string>	_serverNames;
		std::string					_root;
		std::vector<std::string>	_index;
		size_t						_maxClientBody;
		std::vector<int>			_methods;
		std::map<int, std::string>	_errorPages;
		bool						_autoindex;
		bool						_formatOk;

		void	_fillServerInfo(std::string str);
		void	_fillOneInfo(std::string str);
		void	_setListen(std::string str);
		void	_setServerNames(std::string str);
		void	_setRoot(std::string str);
		void	_setIndex(std::string str);
		void	_setMaxClientBody(std::string str);
		void	_setMethods(std::string str);
		void	_setErrorPages(std::string str);
		void	_setAutoIndex(std::string str);

	public:
		Server(void);
		Server(std::string str);
		Server(const Server &server);
		virtual ~Server();

		Server	&operator=(const Server &server);

		// member functions
		bool	wellFormatted(void) const;
		
		static const int	nbKeywords = 8;
		static std::string	keywords[nbKeywords];
};

std::ostream	&operator<<(std::ostream &o, const Server &server);

#endif