/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:41:30 by lle-briq          #+#    #+#             */
/*   Updated: 2022/06/02 17:48:41 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Client::Client(void) : _fd(-1), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_tmpChunked(tmpfile()), _fdTmpChunked(fileno(_tmpChunked)), _response(HTTPResponse()),
	_header(HTTPHeader()), _status(Status()), _isFirstLine(true), _isQuery(false),
	_recvHeader(false), _method(BAD_METHOD), _cgiBody(""), _headerSize(0), _totSize(0),
	_isContentLen(false), _recvBlockSize(false), _readBlock(0), _blockSize(0)
{
	return ;
}

Client::Client(int fd) : _fd(fd), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_tmpChunked(tmpfile()), _fdTmpChunked(fileno(_tmpChunked)), _response(HTTPResponse()),
	_header(HTTPHeader()), _status(Status()), _isFirstLine(true), _isQuery(false),
	_recvHeader(false), _method(BAD_METHOD), _cgiBody(""), _headerSize(0), _totSize(0),
	_isContentLen(false), _recvBlockSize(false), _readBlock(0), _blockSize(0)
{
	return ;
}


Client::Client(const Client &client)
{
	*this = client;
}

Client::~Client()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

Client	&Client::operator=(const Client &client)
{
	if (this != &client)
	{
		_fd = client._fd;
		_request = client._request;
		_tmp = client._tmp;
		_fdTmp = client._fdTmp;
		_tmpChunked = client._tmpChunked;
		_fdTmpChunked = client._fdTmpChunked;
		_response = client._response;
		_header = client._header;
		_status = client._status;
		_isFirstLine = client._isFirstLine;
		_env = client._env;
		_isQuery = client._isQuery;
		_recvHeader = client._recvHeader;
		_method = client._method;
		_cgiBody = client._cgiBody;
		_headerSize = client._headerSize;
		_totSize = client._totSize;
		_recvBlockSize = client._recvBlockSize;
		_readBlock = client._readBlock;
		_blockSize = client._blockSize;
	}
	return (*this);
}

/*
**		MEMBER FUNCTIONS
*/

std::string		Client::getRequest(void) const
{
	return (_request);
}

int				Client::getFd(void) const
{
	return (_fd);
}

FILE			*Client::getTmp(void) const
{
	return (_tmp);
}

int				Client::getFdTmp(void) const
{
	return (_fdTmp);
}

FILE			*Client::getTmpChunked(void) const
{
	return (_tmpChunked);
}

int				Client::getFdTmpChunked(void) const
{
	return (_fdTmpChunked);
}

HTTPResponse	&Client::getResponse(void)
{
	return (_response);
}

HTTPHeader		&Client::getHeader(void)
{
	return (_header);
}

Status			&Client::getStatus(void)
{
	return (_status);
}

bool			Client::isFirstLine(void) const
{
	return (_isFirstLine);
}

bool			Client::hasRecvHeader(void) const
{
	return (_recvHeader);
}

int				Client::getMethod(void) const
{
	return (_method);
}

size_t			Client::getBodySize(void) const
{
	if (_totSize >= _headerSize)
		return ((size_t)(_totSize - _headerSize));
	return (0);
}

size_t			Client::getHeaderSize(void) const
{
	return (_headerSize);
}

size_t			Client::getTotSize(void) const
{
	return (_totSize);
}

void			Client::addRecv(char *buf, int len)
{
	if (!_header.isChunkedEncoded() && _recvHeader &&_method == POST
		&& getBodySize() + len > _header.getContentLenSize())
	{
		len = (size_t)(_header.getContentLenSize() - getBodySize());
		buf[len] = '\0';
	}
	write(_fdTmp, buf, len);
	_totSize += len;
	_request += buf;
}

void			Client::setHeaderSize(size_t size)
{
	_headerSize = size;
	_recvHeader = true;
	_readBlock = size;
}

void			Client::changeFirstLine(void)
{
	_isFirstLine = false;
}

void			Client::setMethod(int method)
{
	_method = method;
}

void			Client::updateMethod(void)
{
	_method = getMethodNb(_header.getMethod());
}

void			Client::setIsQueryString(bool set)
{
	_isQuery = set;
}

void			Client::setCgiBody(std::string str)
{
	_cgiBody = str;
}

bool			Client::isQueryString(void) const
{
	return (_isQuery);
}

void			Client::setEnv(std::string envp)
{
	_env[envp];
}

void			Client::setEnvValue(std::string envp, std::string value)
{
	_env[envp] = value;
}

void			Client::unsetEnv(void)
{
	_env.clear();
}

mapStr			Client::getEnv( void ) const
{	
	return (_env);
}

std::string		Client::getEnvValue(std::string envp)
{	
	return (_env[envp]);
}

size_t			Client::getEnvSize(void) const
{
	return (_env.size());
}


std::string		Client::getCgiBody(void) const
{
	return (_cgiBody);
}

void			Client::setIsContentLen(std::string cgiBody)
{
	int	found;

	found = cgiBody.find("Content-Length:");
	if (found < 0)
		this->_isContentLen = false;
	else
		this->_isContentLen = true;
}

bool			Client::getIsContentLen(void) const
{
	return (_isContentLen);
}

void			Client::clear(bool reopen)
{
	fclose(_tmp);
	if (reopen)
	{
		_tmp = tmpfile();
		_fdTmp = fileno(_tmp);
	}

	fclose(_tmpChunked);
	if (reopen)
	{
		_tmpChunked = tmpfile();
		_fdTmpChunked = fileno(_tmpChunked);
	}
	
	_isFirstLine = true;
	_isQuery = false;
	_recvHeader = false;
	_method = BAD_METHOD;

	_request.clear();
	_env.clear();
	_header.clear();
	_response.clear();
	_status.clear();
	_env.clear();
	_cgiBody = "";
	_headerSize = 0;
	_totSize = 0;
	_recvBlockSize = false;
	_readBlock = 0;
	_blockSize = 0;
}

bool	Client::hasRecvBlockSize(void) const
{
	return (_recvBlockSize);
}

size_t	Client::getReadBlock(void) const
{
	return (_readBlock);
}

size_t	Client::getBlockSize(void) const
{
	return(_blockSize);
}

void	Client::setRecvBlockSize(bool b)
{
	_recvBlockSize = b;
}

void	Client::setReadBlock(size_t size)
{
	_readBlock = size;
}

void	Client::setBlockSize(size_t size)
{
	_blockSize = size;
}

bool	Client::isBlockEnd(size_t cur)
{
	size_t	size;

	size = cur - _readBlock;
	if (size >= _blockSize + 2)
		_readBlock = _readBlock + _blockSize + 2;
	return (size >= _blockSize + 2);
}
