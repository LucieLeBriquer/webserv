#include "usefull.hpp"
//#include "../../includes/usefull.hpp"

void HTTPResponse::STATUS::err4xx(int type)
{
	std::string stype = std::to_string(type);
	this->_code = "40" + stype;
	if ( type == 0 )
		this->_code += " Bad Request";
	if ( type == 4 )
		this->_code += " Not Found";
	if ( type == 5 )
		this->_code += " Method Not Allowed";
//	std::cout << "Error " << this->_code << std::endl;
}

void HTTPResponse::STATUS::status2xx(int type)
{
	std::string stype = std::to_string(type);
	this->_code = "20" + stype;
	if ( type == 0 )
	{
		this->_code += " OK";
	std::cout << "Error " << this->_code << std::endl;
	}
	std::cout << "Erro =" << this->_code << std::endl;
}

void HTTPResponse::STATUS::err5xx(int type)
{
	std::string stype = std::to_string(type);
	this->_code = "50" + stype;
	if ( type == 0 )
		this->_code += " Bad Request";
//	std::cout << "Error " << this->_code << std::endl;
}

std::string HTTPResponse::STATUS::status(int code, int type)
{
	std::map<int, int> status;

	status[4] = 0;
	status[2] = 1;
	status[5] = 2;
	getStatus[0] = &HTTPResponse::STATUS::err4xx;
	getStatus[1] = &HTTPResponse::STATUS::status2xx;
	getStatus[2] = &HTTPResponse::STATUS::err5xx;

	int x = status[code];
	(this->*(getStatus[x]))(type);
	return this->_code;
}