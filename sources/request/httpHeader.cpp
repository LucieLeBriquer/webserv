/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:26 by masboula          #+#    #+#             */
/*   Updated: 2022/04/12 10:28:08 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpHeader.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

HTTPHeader::HTTPHeader() : _host(""), _contentType(""), _contentLen(""), _useragent(""), _accept("")
{
	getFct[0] = &HTTPRequest::get;
	getFct[1] = &HTTPRequest::post;
	getFct[2] = &HTTPRequest::mdelete;

	this->setFct[0] = &HTTPHeader::setHost;
	this->setFct[1] = &HTTPHeader::setContentLen;
	this->setFct[2] = &HTTPHeader::setUserA;
	this->setFct[3] = &HTTPHeader::setAccept;
	this->setFct[4] = &HTTPHeader::setContentType;

	return ;
}

HTTPHeader::HTTPHeader(const HTTPHeader &header) : HTTPRequest(header)
{
	*this = header;
}

HTTPHeader::~HTTPHeader()
{
	return ;
}


/*
**		OVERLOAD OPERATORS
*/

HTTPHeader	&HTTPHeader::operator=(const HTTPHeader &header)
{
	if (this != &header)
	{
		_host = header._host;
		_contentLen = header._contentLen;
		_useragent = header._useragent;
		_accept = header._accept;
		
		// http request parameters
		_META = header._META;
		_OPTION = header._OPTION;
		_method = header._method;
		_httpv = header._httpv;
		_url = header._url;
		_active = header._active;
		_fLine = header._fLine;
	}
	return (*this);
}

/*
**		SETTERS
*/

void HTTPHeader::setContentLen(std::string value)
{
	this->_contentLen = value;
}

void HTTPHeader::setContentType(std::string value)
{
	this->_contentType = value;
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

/*
**		GETTERS
*/

int HTTPHeader::getContext( void )
{
	return this->_active;
}

std::string	HTTPHeader::getHost(void) const
{
	return ("Host: " + _host + "\r\n");
}

std::string	HTTPHeader::getContentLen(void) const
{
	return ("Content-Length: " + _contentLen + "\r\n");
}

std::string	HTTPHeader::getContentType(void) const
{
	if (this->_contentType == "")
		return "Content-Type: text/html; charset=UTF-8\r\n";
	return ("Content-Type: " + _contentType + "\r\n");
}

std::string	HTTPHeader::getUserAgent(void) const
{
	return ("User-Agent: " + _useragent + "\r\n");
}

std::string	HTTPHeader::getAcceptFile(void) const
{
	return ("Accepted: " + _accept + "\r\n");
}

/*
**		MEMBER FUNCTIONS
*/

std::string HTTPHeader::fillrender()
{
	std::vector<std::string> headers(5);
	std::vector<std::string> content(5);
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it2;
	std::string render;

	headers[0] = getHost();
	headers[1] = getContentLen();
	headers[2] = getUserAgent();
	headers[3] = getAcceptFile();
	headers[4] = getContentType();

	content[0] = _host;
	content[1] = _contentLen;
	content[2] = _useragent;
	content[3] = _accept;
	content[4] = _contentType;

	for (it = headers.begin(), it2 = content.begin(); it != headers.end(); it++, it2++)
	{
		if (*it2 != "")
			render += *it;
	}
	return render;
}

int HTTPHeader::parseMethod(const std::string req, const std::string *methods)
{
	int i;
	if (req == "")
		return -1;
	for (i = 0; req[i]; i++)
	{
		if (!isupper(req[i]))
			return (-1);
	}
	for (int j = 0; j < 4; j++)
	{
		if (!strncmp(req.c_str(), methods[j].c_str(), i))
		{
			return (j);
		}
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

std::string	getHead(std::string buf)
{
	std::string firstLine;

	std::stringstream ssin(buf);
	std::getline(ssin, firstLine, '\r');
	// std::cout <<"fline=["<< firstLine<<"]"<<std::endl;
	return (firstLine);
}

int HTTPHeader::method(std::string buf, Status *code, HTTPResponse *deliver)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	std::string line;
	int i;

	line = getHead(buf);
	std::vector<std::string> request = splitThis(line);
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
int HTTPHeader::header()
{
	if (this->_method == "POST")
	{
		if (this->_contentLen == "" || this->_contentType == "")
			return -1;
	}
	else if (this->_method == "GET")
	{
		if (isCssFile(this->_url))
			this->_contentType = "text/css";
		else if (isPngFile(this->_url))
			this->_contentType = "text/avif";
	
	}
	return 1;
}

int HTTPHeader::fillheader(std::string *buf)
{
	std::string header[5] = {"host:", "content-length:", "user-agent:", "accept:", "content-type:"};
	std::string line;
	int i, j;

	if ((*buf)[0] == '\r' && (*buf)[1] == '\n')
		return -1;
	line = getHead(*buf);
	(*buf).erase(0, line.length() + 2);
	for (i = 0; i < 5; i++)
	{
		if (!strncasecmp(line.c_str(), header[i].c_str(), header[i].length()))
			break;
	}
	if (i == 5)
		return (0);
	j = header[i].length();
	if (line[j] == ' ')
		j++;
	int pos = j;
	while (line[j] != '\n' && line[j] != '\r' && line[j] != ' ')
	    j++;
	int len = j - pos;
	char tmp[len + 1];
	line.copy(tmp, len, pos);
	tmp[len] = '\0';

	this->_active = 1;
	std::string value(tmp);
	(this->*(this->setFct[i]))(value);
	// std::cout << "WIIImIIIIIIIIIIIIIIIIIIIIIII" << value << std::endl;
	return (1);
}
