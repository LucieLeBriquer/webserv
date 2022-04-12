/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:43:07 by masboula          #+#    #+#             */
/*   Updated: 2022/04/12 10:18:09 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "usefull.hpp"

class Socket;
class HTTPHeader;
class HTTPResponse
{
	protected:
		std::string 	_contentLen;
		std::string 	_protocol;
		std::string		_statusCode;
		std::string		_url;
		std::string		_header;
		std::string		_method;
		std::string		_fileName;
		std::string		_location;

	public:
		HTTPResponse(void);
		HTTPResponse(const HTTPResponse &response);
		~HTTPResponse();

		HTTPResponse	&operator=(const HTTPResponse &response);

		std::string	getUrl(void );
		std::string	getMethod( void );
		std::string	getHeader( void );
		std::string	getStatus( void );
		void		rendering( HTTPHeader &header );
		void		rendering( const std::string typeContent, HTTPHeader &header);
		void		rendering( const std::string typeContent, HTTPHeader &header, bool b);
		void		setContentLen( int len );
		std::string	checkUrl(Socket *sock, int sockNbr);
		void		redirect(Socket *sock, int sockNbr);
		int			setStatus(std::string code, std::string str);
		void		statusCode(std::string status, std::string firstLine);
		void		setFileName(const std::string &file);
		std::string	getFileName(void) const;
};

#endif