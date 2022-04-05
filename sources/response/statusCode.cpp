#include "usefull.hpp"

void STATUS::err4xx(int type)
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

void STATUS::status2xx(int type)
{
	std::ostringstream s;
	s << type;
	const std::string stype(s.str());
	this->_code = "20" + stype;
	if ( type == 0 )
		this->_code += " OK";
}

void STATUS::err5xx(int type)
{
	std::ostringstream s;
	s << type;
	const std::string stype(s.str());
	this->_code = "50" + stype;
	if ( type == 0 )
		this->_code += " Bad Request";
}

std::string STATUS::status(int code, int type)
{
	std::map<int, int> status;

	status[4] = 0;
	status[2] = 1;
	status[5] = 2;
	getStatus[0] = &STATUS::err4xx;
	getStatus[1] = &STATUS::status2xx;
	getStatus[2] = &STATUS::err5xx;

	int x = status[code];
	(this->*(getStatus[x]))(type);
	return this->_code;
}