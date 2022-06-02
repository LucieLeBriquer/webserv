/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:59 by masboula          #+#    #+#             */
/*   Updated: 2022/04/20 14:34:37 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "usefull.hpp"

class HTTPRequest
{
	protected:
		std::string _META;		// contenant tous les en-tetes http dispo
		std::string _OPTION;	// les methods prises en charge par le serveur
		std::string _method;	// type de method
		std::string	_httpv;		// version 1.0 ou 1.1
		std::string _url;		// le chemin vers la page demandee
		int			_active;
		std::string _fLine;

	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &request);
		virtual ~HTTPRequest();

		HTTPRequest	&operator=(const HTTPRequest &request);
		std::string		getFirstLine(void);
		std::string		getMethod() const;
		std::string		getUrl() const;
		void			clear(void);
};

#endif