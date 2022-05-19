/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:41:30 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/18 17:28:28 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Client::Client(void) : _fd(-1), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false), _recvHeader(false), _method(BAD_METHOD), _cgiCoprs(""), _headerSize(0),
	_totSize(0)
{
	return ;
}

Client::Client(int fd) : _fd(fd), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false), _recvHeader(false), _method(BAD_METHOD), _cgiCoprs(""), _headerSize(0),
	_totSize(0)
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

void			Client::addRecv(char *buf, int len)
{
	write(_fdTmp, buf, len);
	_request += buf;
	_totSize += len;
	if (isFirstLine())
	{
		if (_request.find("POST") != std::string::npos && _request.find(".php") == std::string::npos)
			_request.replace(_request.find("POST"), 4, "GET");
	}
}

void			Client::setHeaderSize(size_t size)
{
	_headerSize = size;
	_recvHeader = true;
}

void			Client::changeFirstLine(void)
{
	_isFirstLine = false;
}

void			Client::setMethod(int method)
{
	_method = method;
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
}
