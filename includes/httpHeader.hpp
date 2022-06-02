/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 09:54:51 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/24 14:44:55 by lle-briq         ###   ########.fr       */
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
		std::string	_acceptEncoding;
		std::string _encoding;
		std::string	_secFetchDest;
		std::string	_secFetchMode;
		ptr			setFct[8];
		
	public:
		HTTPHeader();
		HTTPHeader(const HTTPHeader &header);
		~HTTPHeader();

		HTTPHeader	&operator=(const HTTPHeader &header);

		int			method(std::string buf, Status &code, HTTPResponse &deliver);
		int			parsePath(const std::string url);
		int 		parseProtocol(const std::string prot);
		int			parseMethod(const std::string cmd);

		int			header(void);
		int			fillheader(std::string &buf);
		int			getContext(void);

		void		setHost(std::string value);
		void		setContentLen(std::string value);
		void		setContentType(std::string value);
		void		setContentTypeResponse(std::string value);
		void		setAcceptEncoding(std::string value);
		void		setAccept(std::string value);
		void		setEncoding(std::string value);
		void		setSecFetchDest(std::string value);
		void		setSecFetchMode(std::string value);

		void		checkContext(void);
		int			isChunked(void);
		bool		isChunkedEncoded(void);

		std::string	getHost(void) const;
		std::string	getMethod(void) const;
		std::string	getContentType(void) const;
		std::string	getContentLen(void) const;
		std::string	getContentLenValue(void) const;
		size_t		getContentLenSize(void) const;
		std::string	getAccept(void) const;
		std::string	getResponseContentType(void) const;
		std::string	getSecFetchDest(void) const;
		std::string getSecFetchMode(void) const;

		void		clear(void);
};

#endif

