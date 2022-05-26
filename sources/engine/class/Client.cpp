/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:41:30 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/25 13:56:39 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Client::Client(void) : _fd(-1), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false), _recvHeader(false), _method(BAD_METHOD), _cgiCoprs(""), _headerSize(0),
	_totSize(0), _isContentLen(false), _recvBlockSize(false), _readBlock(0), _blockSize(0),
	_readPos(0)
{
	return ;
}

Client::Client(int fd) : _fd(fd), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false), _recvHeader(false), _method(BAD_METHOD), _cgiCoprs(""), _headerSize(0),
	_totSize(0), _isContentLen(false), _recvBlockSize(false), _readBlock(0), _blockSize(0),
	_readPos(0)
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
		_response = client._response;
		_header = client._header;
		_status = client._status;
		_isFirstLine = client._isFirstLine;
		_env = client._env;
		_isQuery = client._isQuery;
		_recvHeader = client._recvHeader;
		_method = client._method;
		_cgiCoprs = client._cgiCoprs;
		_headerSize = client._headerSize;
		_totSize = client._totSize;
		_recvBlockSize = client._recvBlockSize;
		_readBlock = client._readBlock;
		_blockSize = client._blockSize;
		_readPos = client._readPos;
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
	if (_method == POST && _recvHeader && getBodySize() + len > _header.getContentLenSize())
	{
		len = (size_t)(_header.getContentLenSize() - getBodySize());
		buf[len] = '\0';
	}
	if (!_header.isChunkedEncoded() && _recvHeader)
		write(_fdTmp, buf, len);
	_totSize += len;
	_request += buf;
}

void			Client::setHeaderSize(size_t size)
{
	_headerSize = size;
	_recvHeader = true;
	_readPos = _headerSize;
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

void			Client::setCgiCoprs(std::string str)
{
	_cgiCoprs = str;
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


std::string		Client::getCgiCoprs(void) const
{
	return (_cgiCoprs);
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

void			Client::clear(void)
{
	std::fclose(_tmp);
	_tmp = tmpfile();
	_fdTmp = fileno(_tmp);
	
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
	_cgiCoprs = "";
	_headerSize = 0;
	_totSize = 0;
	_recvBlockSize = false;
	_readBlock = 0;
	_blockSize = 0;
	_readPos = 0;
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

size_t	Client::getReadPos(void) const
{
	return (_readPos);
}

void	Client::setRecvBlockSize(bool b)
{
	_recvBlockSize = b;
}

void	Client::setReadBlock(size_t size)
{
	_readBlock = size;
}

void	Client::setReadPos(size_t size)
{
	_readPos = size;
}

void	Client::setBlockSize(size_t size)
{
	_blockSize = size;
}

bool	Client::isBlockEnd(void)
{
	size_t	size;

	size = _readPos - _readBlock;
	if (size >= _blockSize + 2)
		_readPos = _readBlock + _blockSize + 2;
	return (size >= _blockSize + 2);
}
