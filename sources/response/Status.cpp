/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:55 by masboula          #+#    #+#             */
/*   Updated: 2022/04/10 10:04:30 by lle-briq         ###   ########.fr       */
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

/*
**		OVERLOAD OPERATOR
*/

Status	&Status::operator=(const Status &status)
{
	if (this != &status)
	{
		_code = status._code;
		for (int i = 0; i < 3; i++)
			getStatus[i] = status.getStatus[i];
	}
	return (*this);
}

/*
**		MEMBER FUNCTIONS
*/

void Status::err4xx(int type)
{
	std::ostringstream s;
	s << type;
	const std::string stype(s.str());

	this->_code = "40" + stype;
	if ( type == 0 )
		this->_code += " Bad Request";
	if ( type == 4 )
		this->_code += " Not Found";
	if ( type == 5 )
		this->_code += " Method Not Allowed";
}

void Status::status2xx(int type)
{
	std::ostringstream s;
	s << type;
	const std::string stype(s.str());
	this->_code = "20" + stype;
	if ( type == 0 )
		this->_code += " OK";
}

void Status::err5xx(int type)
{
	std::ostringstream s;
	s << type;
	const std::string stype(s.str());
	this->_code = "50" + stype;
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
