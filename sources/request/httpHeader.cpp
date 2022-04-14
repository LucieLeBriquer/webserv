/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:26 by masboula          #+#    #+#             */
/*   Updated: 2022/04/14 14:24:13 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpHeader.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

HTTPHeader::HTTPHeader() : _host(""), _contentLen(""), _contentType(""), _contentTypeResponse("text/html"), _accept("")
{
	this->setFct[0] = &HTTPHeader::setHost;
	this->setFct[1] = &HTTPHeader::setContentLen;
	this->setFct[2] = &HTTPHeader::setContentType;
	this->setFct[3] = &HTTPHeader::setAccept;

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
		_contentType = header._contentType;
		_contentTypeResponse = header._contentTypeResponse;
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

void HTTPHeader::setHost(std::string value)
{
	this->_host = value;
}

void HTTPHeader::setContentLen(std::string value)
{
	this->_contentLen = value;
}

void HTTPHeader::setContentType(std::string value)
{
	this->_contentType = value;
}

void HTTPHeader::setContentTypeResponse(std::string value)
{
	this->_contentTypeResponse = value;
}

void HTTPHeader::setAccept(std::string value)
{
	this->_accept = value;
	setContentTypeResponse(_accept.substr(0, _accept.find(',')));
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
	return (_contentType);
}

std::string	HTTPHeader::getResponseContentType(void) const
{
	return ("Content-Type: " + _contentTypeResponse + "\r\n");
}

/*
**		MEMBER FUNCTIONS
*/

std::string HTTPHeader::fillrender()
{
	std::vector<std::string> headers(2);
	std::vector<std::string> content(2);
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it2;
	std::string render;

	headers[0] = getHost();
	headers[1] = getResponseContentType();

	content[0] = _host;
	content[1] = _accept;

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
	{
		_method = methods[i];
		//(this->*(getFct[i]))();
	}
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
		if (this->_contentLen == "")
			return -1;
	}
	return 1;
}

int HTTPHeader::fillheader(std::string *buf)
{
	std::string header[4] = {"host:", "content-length:", "content-type:" ,"accept:"};
	std::string line;
	int			headerSize = 4;
	int i, j;

	if ((*buf)[0] == '\r' && (*buf)[1] == '\n')
		return -1;
	line = getHead(*buf);
	(*buf).erase(0, line.length() + 2);
	for (i = 0; i < headerSize; i++)
	{
		if (!strncasecmp(line.c_str(), header[i].c_str(), header[i].length()))
			break;
	}
	if (i == headerSize)
		return (0);
	j = header[i].length();
	if (line[j] == ' ')
		j++;
	int pos = j;
	while (line[j] != '\n' && line[j] != '\r' && line[j] != ' ')
	    j++;
	this->_active = 1;
	(this->*(this->setFct[i]))(line.substr(pos, line.size() - pos));
	return (1);
}
