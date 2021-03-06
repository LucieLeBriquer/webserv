/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:38 by masboula          #+#    #+#             */
/*   Updated: 2022/05/27 11:01:42 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpRequest.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

HTTPRequest::HTTPRequest() : _META(""), _OPTION(""), _method(""), _httpv(""), _url(""), _active(0), _fLine("")
{
	return ;
}

HTTPRequest::HTTPRequest(const HTTPRequest &request)
{
	*this = request;
}

HTTPRequest::~HTTPRequest()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

HTTPRequest	&HTTPRequest::operator=(const HTTPRequest &request)
{
	if (this != &request)
	{
		_META = request._META;
		_OPTION = request._OPTION;
		_method = request._method;
		_httpv = request._httpv;
		_url = request._url;
		_active = request._active;
		_fLine = request._fLine;
	}
	return (*this);
}

/*
**		MEMBER FUNCTIONS
*/

std::string	HTTPRequest::getFirstLine(void)
{
	this->_fLine = this->_method + " " + this->_url + " " + this->_httpv;
	return this->_fLine;
}

std::string	HTTPRequest::getUrl(void) const
{
	return (_url);
}

std::string	HTTPRequest::getMethod(void) const
{
	return (_method);
}

void		HTTPRequest::clear(void)
{
	_META = "";
	_OPTION = "";
	_method = "";
	_httpv = "";
	_url = "";
	_active = 0;
	_fLine = "";
}
