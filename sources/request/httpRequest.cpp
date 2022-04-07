#include "usefull.hpp"

HTTPRequest::HTTPRequest()
{
	this->_httpv = "HTTP/1.0";
	this->_url = "/";
	this->_method = "NULL";
};

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

std::string	HTTPRequest::getFirstLine( void )
{
	this->_fLine = this->_method + " " + this->_url + " " + this->_httpv;
	return this->_fLine;
}
