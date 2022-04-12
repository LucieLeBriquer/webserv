/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 09:54:51 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/11 18:03:29 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HTTPHEADER_HPP
# define HTTPHEADER_HPP
# include "httpRequest.hpp"
# include "Status.hpp"

class HTTPHeader : public HTTPRequest
{
	friend class HTTPRequest;

	protected:
		std::string	_host;
		std::string	_contentType;
		std::string	_contentLen;
		std::string	_useragent;
		std::string	_accept;

		typedef void (HTTPHeader::*ptr)(std::string);
		ptr			setFct[5];
		
	public:
		HTTPHeader();
		HTTPHeader(const HTTPHeader &header);
		~HTTPHeader();

		HTTPHeader	&operator=(const HTTPHeader &header);

		int		method(std::string buf, Status *code, HTTPResponse *deliver);
		int		parsePath(const std::string url);
		int 	parseProtocol(const std::string prot);
		int		parseMethod(const std::string cmd, const std::string *methods);

		int		header();
		std::string		fillrender( void );
		int		fillheader(std::string *buf);
		int		getContext();
		void	setContentType(std::string value);
		void	setContentLen(std::string value);
		void	setHost(std::string value);
		void	setUserA(std::string value);
		void	setAccept(std::string value);
		void	checkContext();

		std::string	getHost(void) const;
		std::string	getContentType(void) const;
		std::string	getContentLen(void) const;
		std::string	getUserAgent(void) const;
		std::string	getAcceptFile(void) const;
};

#endif

