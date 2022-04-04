#include "usefull.hpp"

//#include "../../includes/usefull.hpp"

std::string HTTPResponse::getHeader( void )
{
	return this->_header;
}

void HTTPResponse::body(int code, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m)
{
	//retour d'erreur
}

void HTTPResponse::response(std::string status, std::string prot, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m)
{
	this->_statusCode = status;
	this->_protocol = prot;
	this->body(code + type, sc, m);
}

void HTTPResponse::header( void )
{
	time_t rawtime = time(0);
	// struct tm * ptm;
	// time ( &rawtime );
  	// ptm = gmtime ( &rawtime );
	std::string time = ctime(&rawtime);
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: text/html; charset=UTF-8\nReferrer-Policy: no-referrer\nContent-Length: 1589\nDate: " + time;
	// this->_header += "Date: "+ ptm->tm_mday + ", " + ptm->tm_mon + ", " + ptm->tm_year + ", " + ptm->tm_hour + ":" + ptm->tm_min + ":" + ptm->tm_sec + " GMT\n"; 
}