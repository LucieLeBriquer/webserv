/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:34:40 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/19 11:30:39 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "usefull.hpp"
# include "httpResponse.hpp"
# include "httpRequest.hpp"
# include "httpHeader.hpp"

class Client
{
	private:
		int				_fd;
		std::string		_request;
		FILE*			_tmp;
		int				_fdTmp;
		HTTPResponse	_response;
		HTTPHeader		_header;
		Status			_status;
		bool			_isFirstLine;
		mapStr			_env;
		bool			_isQuery;
		bool			_recvHeader;
		int				_method;
		std::string		_cgiCoprs;
		size_t			_headerSize;
		size_t			_totSize;
		
	public:
		Client(void);
		Client(int fd);
		Client(const Client &client);
		virtual ~Client();

		Client	&operator=(const Client &client);

		std::string		getRequest(void) const;
		int				getFd(void) const;
		FILE			*getTmp(void) const;
		int				getFdTmp(void) const;
		HTTPResponse	&getResponse(void);
		HTTPHeader		&getHeader(void);
		Status			&getStatus(void);
		bool			isFirstLine(void) const;
		bool			hasRecvHeader(void) const;
		int				getMethod(void) const;
		size_t			getBodySize(void) const;

		mapStr			getEnv(void) const;
		std::string		getEnvValue(std::string envp);
		void			setEnv(std::string envp);
		void			setEnvValue(std::string envp, std::string value);
		size_t			getEnvSize(void) const;
		void			unsetEnv(void);

		void			setIsQueryString(bool set);
		bool			isQueryString(void) const;

		std::string		getCgiCoprs(void) const;
		void			setCgiCoprs(std::string str);

		void			addRecv(char *buf, int len);
		void			setHeaderSize(size_t size);
		void			changeFirstLine(void);
		void			setMethod(int method);

		void			clear();
};

#endif
