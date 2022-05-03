/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 09:54:51 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/20 14:33:38 by masboula         ###   ########.fr       */
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
		typedef void (HTTPHeader::*ptr)(std::string);

		std::string	_host;
		std::string	_contentLen;
		std::string	_contentType;
		std::string	_contentTypeResponse;
		std::string	_accept;
		ptr			setFct[4];
		
	public:
		HTTPHeader();
		HTTPHeader(const HTTPHeader &header);
		~HTTPHeader();

		HTTPHeader	&operator=(const HTTPHeader &header);

		int			method(std::string buf, Status *code, HTTPResponse *deliver);
		int			parsePath(const std::string url);
		int 		parseProtocol(const std::string prot);
		int			parseMethod(const std::string cmd, const std::string *methods);

		int			header(std::string str);
		int			fillheader(std::string *buf);
		int			getContext(void);

		void		setHost(std::string value);
		void		setContentLen(std::string value);
		void		setContentType(std::string value);
		void		setContentTypeResponse(std::string value);
		void		setAccept(std::string value);

		void		checkContext(void);

		std::string	getHost(void) const;
		std::string	getMethod(void) const;
		std::string	getContentType(void) const;
		std::string	getContentLen(void) const;
		std::string	getAccept(void) const;
		std::string	getResponseContentType(void) const;
};

#endif

