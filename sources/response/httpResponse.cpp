#include "usefull.hpp"

//#include "../../includes/usefull.hpp"

std::string HTTPResponse::getHeader( void )
{
	return this->_header;
}

int HTTPHeader::getContext( void )
{
	return this->_active;
}


void HTTPHeader::checkContext()
{
//	if (this->_host != NULL && this->_host != "127.0.0.2")
		//this->_statusCode = 404;
}

// void HTTPResponse::body(int code, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m)
// {
// 	//retour d'erreur
// }

void HTTPResponse::statusCode(std::string status, std::string prot)
{
	this->_statusCode = status;
	this->_protocol = prot;
//	this->body(, sc, m);
}

void HTTPResponse::rendering( void )
{
	time_t rawtime = time(0);
	struct tm *ptm = localtime(&rawtime);
	// time ( &rawtime );
  	// ptm = gmtime ( &rawtime );
	std::string time = ctime(&rawtime);
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: text/html; charset=UTF-8\nReferrer-Policy: no-referrer\nContent-Length: 1589\nDate: ";
	this->_header += ptm->tm_mday + ", " + (1 + ptm->tm_mon) + ", " + ptm->tm_year + ", " + (5 + ptm->tm_hour) + ":" + (30+ ptm->tm_min) + ":" + ptm->tm_sec + " GMT\n"; 
}