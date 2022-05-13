/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 13:41:30 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/12 16:44:07 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Client::Client(void) : _fd(-1), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false)
{
	return ;
}

Client::Client(int fd) : _fd(fd), _request(""), _tmp(tmpfile()), _fdTmp(fileno(_tmp)),
	_response(HTTPResponse()), _header(HTTPHeader()), _status(Status()), _isFirstLine(true),
	_isQuery(false)
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

void			Client::addRecv(char *buf, int len)
{
	write(_fdTmp, buf, len);
	_request += buf;
}

void			Client::changeFirstLine(void)
{
	_isFirstLine = false;
}

void			Client::clear(void)
{
	std::fclose(_tmp);
	_tmp = tmpfile();
	_fdTmp = fileno(_tmp);
	
	_isFirstLine = true;	
	_isQuery = false;

	_request.clear();
	_env.clear();
	_header.clear();
	_response.clear();
	_status.clear();
	_env.clear();
}

void			Client::setIsQueryString(bool set)
{
	_isQuery = set;
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

std::string		Client::getEnvValue( std::string envp )
{	
	return (_env[envp]);
}

size_t			Client::getEnvSize(void) const
{
	return this->_env.size();
}
