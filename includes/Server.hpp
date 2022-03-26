/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:54 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 15:21:01 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		void	_setWrontFormat(void);

	public:
		Server(void);
		Server(std::string str);
		Server(const Server &server);
		virtual ~Server();

		Server	&operator=(const Server &server);

		// member functions
		bool	wellFormatted(void) const;
		
		// getters
		std::string	getHost(void) const;
		int			getPort(void) const;
		vecStr		getServerNames(void) const;
		std::string	getRoot(void) const;
		vecStr		getIndex(void) const;
		size_t		getMaxClientBody(void) const;
		vecInt		getMethods(void) const;
		mapErr		getErrorPages(void) const;
		bool		getAutoIndex(void) const;

		// static
		static const int	nbKeywords = 8;
		static std::string	keywords[nbKeywords];
};

std::ostream	&operator<<(std::ostream &o, const Server &server);

typedef void (Server::*setFunc)(vecStr words);

#endif
