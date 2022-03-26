#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"
# include <map>

typedef	std::vector<Location>		vecLoc;

class Server
{
	private:
		vecLoc		_locations;
		std::string	_host;
		int			_port;
		vecStr		_serverNames;
		std::string	_root;
		vecStr		_index;
		size_t		_maxClientBody;
		vecInt		_methods;
		mapErr		_errorPages;
		bool		_autoindex;
		bool		_formatOk;

		void	_fillServerInfo(std::string str);
		void	_fillOneInfo(std::string str);

		void	_setListen(vecStr words);
		void	_setServerNames(vecStr words);
		void	_setRoot(vecStr words);
		void	_setIndex(vecStr words);
		void	_setMaxClientBody(vecStr words);
		void	_setMethods(vecStr words);
		void	_setErrorPages(vecStr words);
		void	_setAutoIndex(vecStr words);

		int		_keywordNumber(std::string str);

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

typedef void (Server::*setFunc)(vecStr words);

#endif
