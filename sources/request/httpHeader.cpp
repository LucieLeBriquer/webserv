#include "usefull.hpp"

//#include "../../includes/usefull.hpp"

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

int HTTPRequest::header(std::string buf, HTTPHeader *h)
{
	std::string header[3] = {"host:", "user-agent:", "accept:"};
	
	h->setFct[0] = &HTTPHeader::setHost;
	h->setFct[1] = &HTTPHeader::setUserA;
	h->setFct[2] = &HTTPHeader::setAccept;

	int i, j;

	for (i = 0; i < 3; i++)
	{
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
	return (1);
}