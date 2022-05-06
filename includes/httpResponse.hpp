/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:43:07 by masboula          #+#    #+#             */
/*   Updated: 2022/05/06 11:39:34 by masboula         ###   ########.fr       */
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
		std::string 	_options;
		std::string 	_contentLen;
		std::string 	_protocol;
		std::string		_statusCode;
		std::string		_url;
		std::string		_header;
		std::string		_method;
		std::string		_fileName;
		std::string		_location;
		int				_statusNb;
		int				_redir;
		bool			_needAutoindex;
		int				_chunked;
		int				_max_size_c;

		std::string		_returnErrPage(Socket &sock, int sockNbr);
		std::string		_returnSetErrPage(Socket &sock, int sockNbr, std::string code,
										std::string str, HTTPHeader &header);
		std::string		_manageDirectory(Socket &sock, int sockNbr, HTTPHeader &header);
		//std::string		_manageRegularFile(Socket &sock, int sockNbr, HTTPHeader &header);

	public:
		HTTPResponse(void);
		HTTPResponse(const HTTPResponse &response);
		~HTTPResponse();

		HTTPResponse	&operator=(const HTTPResponse &response);

		std::string	getUrl(void) const;
		std::string	getProtocol(void) const;
		std::string	getContentLen(void) const;
		std::string	getMethod(void) const;
		std::string	getHeader(void) const;
		std::string	getStatus(void) const;
		std::string	getFileName(void) const;
		int			getStatusNb(void) const;
		int			getMethodNbr(void) const;
		int			getRedir(void) const;
		int			getMaxSizeC(void) const;
		bool		getNeedAutoindex(void) const;

		void		rendering(HTTPHeader &header);

		void		setContentLen(int len);
		void		setStatusErr(int nbErr);
		int			setStatus(std::string code, std::string str, HTTPHeader &header);
		void		setFileName(const std::string &file);
		void		setStatusNb(int nb);
		void		setUrl(const std::string &url);
		void		setMethod(const std::string &method);
		void		setRedir(int r);

		int			isChunked(void);
		std::string	checkUrl(Socket &sock, int sockNbr, HTTPHeader &header);
		std::string	redirect(Socket &sock, int sockNbr, HTTPHeader &header);
		void		statusCode(std::string status, std::string firstLine);
};

#endif