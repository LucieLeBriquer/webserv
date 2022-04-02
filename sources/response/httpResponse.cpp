//#include "usefull.hpp"
#include "../../includes/usefull.hpp"

std::string HTTPResponse::getHeader( void )
{
	return this->_header;
}

void HTTPResponse::response(int code, int type, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m)
{
	this->_statusCode = sc->status(code, type);
	//this->_ = sc->get;
	this->_protocol = m->getProtocol();
	std::cout << "code here = " << this->_statusCode << std::endl; 
}

void HTTPResponse::header( void )
{
	std::cout << "code = " << this->_statusCode << std::endl;
	this->_header = this->_protocol + ' ' + this->_statusCode + "OK";

}