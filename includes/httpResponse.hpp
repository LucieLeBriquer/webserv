/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:43:07 by masboula          #+#    #+#             */
/*   Updated: 2022/04/10 23:27:51 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "usefull.hpp"

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

	public:
		HTTPResponse(void);
		HTTPResponse(const HTTPResponse &response);
		~HTTPResponse();

		HTTPResponse	&operator=(const HTTPResponse &response);

		std::string	getUrl(void );
		std::string	getMethod( void );
		std::string	getHeader( void );
		void		rendering( void );
		void		rendering( const std::string typeContent );
		void		rendering( const std::string typeContent, bool b);
		void		setContentLen( int len );
		std::string	checkUrl();
		int HTTPResponse::setStatus(std::string code, std::string str);
		void		statusCode(std::string status, std::string firstLine);
		void		setFileName(const std::string &file);
		std::string	getFileName(void) const;
};

#endif