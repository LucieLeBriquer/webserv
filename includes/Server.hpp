/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:54 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 17:19:57 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"
# include <map>

typedef	std::vector<Location>		vecLoc;

class Server : public Block
{
	private:
		vecLoc		_locations;
		std::string	_host;
		int			_port;
		vecStr		_serverNames;
		
		bool		_hostSet;
		bool		_serverNamesSet;

		void	_fillServerInfo(std::string str);
		void	_fillOneInfo(std::string str);

		void	_setListen(vecStr words);
		void	_setServerNames(vecStr words);

		int		_keywordNumber(std::string str);

	public:
		Server(void);
		Server(std::string str);
		Server(const Server &server);
		virtual ~Server();

		Server	&operator=(const Server &server);

		// getters
		const vecLoc		getLocations(void) const;
		const std::string	getHost(void) const;
		int					getPort(void) const;
		const vecStr		getServerNames(void) const;
		
		bool				isServerNamesSet(void) const;
		bool				isHostSet(void) const;

		// useful methods
		int			configFromUrl(const std::string &str) const;
		std::string	getRealUrl(const std::string &str) const;

		// static
		static const int	nbKeywords = 9;
		static const std::string	keywords[nbKeywords];
};

std::ostream	&operator<<(std::ostream &o, const Server &server);

typedef void (Server::*setFunc2)(vecStr words);

#endif
