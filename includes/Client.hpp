/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:34:40 by lle-briq          #+#    #+#             */
/*   Updated: 2022/06/01 17:42:07 by lle-briq         ###   ########.fr       */
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
		FILE*			_tmpChunked;
		int				_fdTmpChunked;
		HTTPResponse	_response;
		HTTPHeader		_header;
		Status			_status;
		bool			_isFirstLine;
		mapStr			_env;
		bool			_isQuery;
		bool			_recvHeader;
		int				_method;
		std::string		_cgiBody;
		size_t			_headerSize;
		size_t			_totSize;
		bool			_isContentLen;
		bool			_recvBlockSize;
		size_t			_readBlock;
		size_t			_blockSize;
		
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
		FILE			*getTmpChunked(void) const;
		int				getFdTmpChunked(void) const;
		HTTPResponse	&getResponse(void);
		HTTPHeader		&getHeader(void);
		Status			&getStatus(void);
		bool			isFirstLine(void) const;
		bool			hasRecvHeader(void) const;
		int				getMethod(void) const;
		size_t			getBodySize(void) const;
		size_t			getHeaderSize(void) const;
		size_t			getTotSize(void) const;

		mapStr			getEnv(void) const;
		std::string		getEnvValue(std::string envp);
		void			setEnv(std::string envp);
		void			setEnvValue(std::string envp, std::string value);
		size_t			getEnvSize(void) const;
		void			unsetEnv(void);

		void			setIsQueryString(bool set);
		bool			isQueryString(void) const;

		std::string		getCgiBody(void) const;
		void			setCgiBody(std::string str);

		void			addRecv(char *buf, int len);
		void			setHeaderSize(size_t size);
		void			changeFirstLine(void);
		void			setMethod(int method);
		void			updateMethod(void);

		void			setIsContentLen(std::string cgiBody);
		bool			getIsContentLen(void) const;

		bool			hasRecvBlockSize(void) const;
		size_t			getReadBlock(void) const;
		size_t			getBlockSize(void) const;
		void			setRecvBlockSize(bool b);
		void			setReadBlock(size_t size);
		void			setBlockSize(size_t size);
		bool			isBlockEnd(size_t cur);

		void			clear(bool reopen = true);
};

#endif
