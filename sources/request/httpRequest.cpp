#include "usefull.hpp"

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

int HTTPRequest::header(std::string buf, HTTPHeader *h)
{
	std::string header[4] = {"host:", "content-length:", "user-agent:", "accept:"};
	
	h->setFct[0] = &HTTPHeader::setHost;
	h->setFct[1] = &HTTPHeader::setContentLen;
	h->setFct[2] = &HTTPHeader::setUserA;
	h->setFct[3] = &HTTPHeader::setAccept;

	int i, j;

	for (i = 0; i < 3; i++)
	{
		std::cout << "--> buf = |" << buf.c_str() << "| ------ header[" << i << "] = |" << header[i].c_str() << "| ------ header[" << i << "].length() = " << header[i].length() << "<--" << std::endl;
		if (!strncasecmp(buf.c_str(), header[i].c_str(), header[i].length()))
			break;
	}
	if (i == 3)
		return (0);
	j = header[i].length();
	if (buf[j] == ' ')
		j++;
	int pos = j;
	while (buf[j] != '\n' && buf[j] != '\r' && buf[j] != ' ')
	    j++;
	int len = j - pos;
	char tmp[len + 1];
	buf.copy(tmp, len, pos);
	tmp[len] = '\0';

	h->_active = 1;
	std::string value(tmp);
	(h->*(h->setFct[i]))(value);
	std::cout << "REEEETTTTUUURRRNNNNN" << std::endl;
	return (1);
}