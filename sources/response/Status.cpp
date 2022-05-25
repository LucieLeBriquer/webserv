/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:55 by masboula          #+#    #+#             */
/*   Updated: 2022/04/11 12:06:55 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Status.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Status::Status() : _code("")
{
	getStatus[0] = &Status::err4xx;
	getStatus[1] = &Status::status2xx;
	getStatus[2] = &Status::err5xx;
}

Status::Status(const Status &status) : HTTPResponse(status)
{
	*this = status;
}

Status::~Status()
{
	return ;
}

void	Status::clear(void)
{
	(*this).HTTPResponse::clear();
	_code = "";
}

/*
**		OVERLOAD OPERATOR
*/

Status	&Status::operator=(const Status &status)
{
	if (this != &status)
	{
		(*this).HTTPResponse::operator=(status);
		_code = status._code;
		for (int i = 0; i < 3; i++)
			getStatus[i] = status.getStatus[i];
	}
	return (*this);
}

/*
**		MEMBER FUNCTIONS
*/

static std::string	get2Dig(int type)
{
	if (type >= 0 && type <= 9)
		return ("0" + toString(type));
	return (toString(type));
}

void Status::err4xx(int type)
{
	this->_code = "4" + get2Dig(type) + " ";
	if (type == 0)
		this->_code += "Bad Request";
	else if (type == 4)
		this->_code += "Not Found";
	else if (type == 5)
		this->_code += "Method Not Allowed";
	else if (type == 13)
		this->_code += "Request Entity Too Large";
}

void Status::status2xx(int type)
{
	this->_code = "20" + toString(type);
	if ( type == 0 )
		this->_code += " OK";
}

void Status::err5xx(int type)
{
	this->_code = "50" + toString(type);
	if ( type == 0 )
		this->_code += " Bad Request";
}

std::string Status::status(int code, int type)
{
	std::map<int, int> status;

	status[4] = 0;
	status[2] = 1;
	status[5] = 2;

	int x = status[code];
	(this->*(getStatus[x]))(type);
	return this->_code;
}
