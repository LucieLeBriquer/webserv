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

int HTTPHeader::parseMethod(const std::string req, const std::string *methods)
{
	int i;
	for (i = 0; req[i]; i++)
	{
		if (!isupper(req[i]))
			return (-1);
	}
	for (int j = 0; j < 4; j++)
	{
		if (!strncmp(req.c_str(), methods[j].c_str(), i))
			return (j);
	}
	return (-1);
}

int HTTPHeader::parsePath(const std::string url)
{
	if (url == "")
		return 1;
	if (url[0] != '/')
		return 0;
	else
		this->_url = url;
	return 1;
}

int HTTPHeader::parseProtocol(const std::string protocol)
{
	if (protocol == "")
		return 1;
	if (strncmp(protocol.c_str(), "HTTP/1.0", 8) && strncmp(protocol.c_str(), "HTTP/1.1", 8))
		return 0;
	this->_httpv = protocol;
	return 1;
}

int HTTPHeader::method(std::string buf, STATUS *code, HTTPResponse *deliver)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	
	int i;

	getFct[0] = &HTTPRequest::get;
	getFct[1] = &HTTPRequest::post;
	getFct[2] = &HTTPRequest::mdelete;

	std::vector<std::string> request = splitThis(buf);
	std::vector<std::string>::iterator it;
	int arg = 0;
	for (it = request.begin(); it != request.end(); it++)
	{
		if (*it != "")
			arg++;
	}
	this->_httpv = "HTTP/1.0";
	this->_url = "/";
	this->_method = "NULL";
	if ((i = this->parseMethod(request[0], methods)) == -1)
	{
		deliver->statusCode(code->status(4, 5), this->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	else
		(this->*(getFct[i]))();
	if (!this->parsePath(request[1]))
	{
		deliver->statusCode(code->status(4, 4), this->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	if (!this->parseProtocol(request[2]))
	{
		deliver->statusCode(code->status(4, 0), this->getFirstLine());
		return -1;
	}
	deliver->statusCode(code->status(2, 0), this->getFirstLine());
	if (arg != 3)
		return -1;
	return 1;
}

int HTTPHeader::header(std::string buf)
{
	std::string header[4] = {"host:", "content-length", "user-agent:", "accept:"};
	
	this->setFct[0] = &HTTPHeader::setHost;
	this->setFct[1] = &HTTPHeader::setContentLen;
	this->setFct[2] = &HTTPHeader::setUserA;
	this->setFct[3] = &HTTPHeader::setAccept;

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

	this->_active = 1;
	std::string value(tmp);
	(this->*(this->setFct[i]))(value);
	return (1);
}