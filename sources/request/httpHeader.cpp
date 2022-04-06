#include "usefull.hpp"

void HTTPHeader::setContentLen(std::string value)
{
	this->_contentLen = value;
}

void HTTPHeader::setHost(std::string value)
{
	this->_host = value;
}

void HTTPHeader::setUserA(std::string value)
{
	this->_useragent = value;
}

void HTTPHeader::setAccept(std::string value)
{
	this->_accept = value;
}

int HTTPHeader::getContext( void )
{
	return this->_active;
}