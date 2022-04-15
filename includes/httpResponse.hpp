/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:43:07 by masboula          #+#    #+#             */
/*   Updated: 2022/04/15 15:10:25 by lle-briq         ###   ########.fr       */
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
		int				_statusNb;
		int				_redir;
		bool			_needAutoindex;

		std::string		_returnErrPage(Socket &sock, int sockNbr);
		std::string		_returnSetErrPage(Socket &sock, int sockNbr, std::string code,
										std::string str, HTTPHeader &header);

	public:
		HTTPResponse(void);
		HTTPResponse(const HTTPResponse &response);
		~HTTPResponse();

		HTTPResponse	&operator=(const HTTPResponse &response);

		std::string	getUrl(void) const;
		std::string	getMethod(void) const;
		std::string	getHeader(void) const;
		std::string	getStatus(void) const;
		std::string	getFileName(void) const;
		int			getStatusNb(void) const;
		int			getMethodNbr(void) const;

		void		rendering(HTTPHeader &header);

		void		setContentLen(int len);
		void		setStatusErr(int nbErr);
		int			setStatus(std::string code, std::string str, HTTPHeader &header);
		void		setFileName(const std::string &file);
		void		setStatusNb(int nb);
		void		setMethod(const std::string &method);

		std::string	checkUrl(Socket &sock, int sockNbr, HTTPHeader &header);
		std::string	redirect(Socket &sock, int sockNbr, std::string filename);
		void		statusCode(std::string status, std::string firstLine);
};

#endif