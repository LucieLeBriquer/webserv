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
//	std::cout << "code here = " << this->_statusCode << std::endl; 
}

void HTTPResponse::header( void )
{
	time_t rawtime = time(0);
	// struct tm * ptm;
	// time ( &rawtime );
  	// ptm = gmtime ( &rawtime );
	std::string time = ctime(&rawtime);

//	std::cout << "code = " << this->_statusCode << std::endl;
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: text/html; charset=UTF-8\nReferrer-Policy: no-referrer\nContent-Length: 1589\nDate: " + time;
	// char _tmp[this->_header.length() + 1];
	// int i = 0;
	// while (_header[i] != '\0')
	// {
	// 	_tmp[i] = _header[i];
	// 	i++;
	// }
	// _tmp[i] = '\0';
	// this->_cheader = strcpy(_cheader, _tmp);
	// std::cout << "_tmp: " << _tmp << std::endl;
	// this->_header += "Date: "+ ptm->tm_mday + ", " + ptm->tm_mon + ", " + ptm->tm_year + ", " + ptm->tm_hour + ":" + ptm->tm_min + ":" + ptm->tm_sec + " GMT\n"; 
}