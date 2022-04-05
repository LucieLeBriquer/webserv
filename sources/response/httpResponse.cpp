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

std::string HTTPResponse::checkUrl()
{
	std::string filename("html");
	std::stringstream ss;
	int status;

	ss << this->_statusCode;
	ss >> status;
	if (status > 299 )
	{
		this->_url = "/404.html";
	}
	if (this->_url == "/")
		this->_url = "/index.html";
	return filename += this->_url;
}

// void HTTPResponse::body(int code, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m)
// {
// 	//retour d'erreur
// }

void HTTPResponse::setContentLen(int len)
{
	std::ostringstream s;
	s << len;
	const std::string tot_len(s.str());
	this->_contentLen = tot_len;
}

void HTTPResponse::statusCode(std::string status, std::string firstLine)
{
	std::vector<std::string> line = splitThis(firstLine);

	this->_statusCode = status;
	this->_protocol = line[2];
	this->_url = line[1];
//	this->body(, sc, m);
}

void HTTPResponse::rendering( void )
{
	time_t rawtime;
	time(&rawtime);
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: text/html; charset=UTF-8\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nDate: " + ctime(&rawtime); 
	std::cout << this->_header << std::endl;
}