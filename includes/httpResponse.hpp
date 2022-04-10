/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:43:07 by masboula          #+#    #+#             */
/*   Updated: 2022/04/08 12:52:27 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "httpRequest.hpp"

class STATUS;

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
		void		setStatus(std::string code, std::string str);
		void		statusCode(std::string status, std::string firstLine);
		void		setFileName(const std::string &file);
		std::string	getFileName(void) const;
};

class STATUS: public HTTPResponse
{
	friend class HTTPResponse;
	protected:
		std::string	_code;
		typedef void (STATUS::*ptr)(int);
		ptr			getStatus[3];
	public:
		STATUS();
		~STATUS(){};
		std::string status(int code, int type);
		void        err4xx(int type);
		void        status2xx(int type);
		void        err5xx( int type);
};

#endif