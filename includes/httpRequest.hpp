/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:59 by masboula          #+#    #+#             */
/*   Updated: 2022/04/10 09:55:11 by lle-briq         ###   ########.fr       */
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
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[3];

	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &request);
		~HTTPRequest();

		HTTPRequest	&operator=(const HTTPRequest &request);
		std::string		getFirstLine(void);
		std::string		getMethod();
		std::string		getUrl();
		void			get(void);
		void			post(void);
		void			mdelete(void);

};

#endif