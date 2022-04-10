/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/10 08:52:22 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName("")
{
	if (LOG)
		std::cout << YELLOW << "[HTTPResponse]" << END << " default constructor" << std::endl;

}

HTTPResponse::HTTPResponse(const HTTPResponse &response)
{
	if (LOG)
		std::cout << YELLOW << "[HTTPResponse]" << END << " copy constructor" << std::endl;
	*this = response;
}

HTTPResponse::~HTTPResponse()
{
	if (LOG)
		std::cout << RED << "[HTTPResponse]" << END << " destructor" << std::endl;
}

HTTPResponse	&HTTPResponse::operator=(const HTTPResponse &response)
{
	if (this != &response)
	{
		_contentLen = response._contentLen;
		_protocol = response._protocol;
		_statusCode = response._statusCode;
		_url = response._url;
		_header = response._header;
		_method = response._method;
		_fileName = response._fileName;
	}
	std::cout << "[" << _fileName << "] [" << response._fileName << "]" << std::endl;
	return (*this);
}


std::string HTTPResponse::getMethod( void )
{
	return this->_method;
}

std::string HTTPResponse::getUrl( void )
{
	return this->_url;
}

std::string HTTPResponse::getHeader( void )
{
	return this->_header;
}

std::string	HTTPResponse::getFileName(void) const
{
	return (_fileName);
}

void HTTPResponse::setStatus(std::string code, std::string str)
{
	std::stringstream ss;
	std::map<int, std::string> getStatus;
	int status;
	
	ss << code;
	ss >> status;

	getStatus[400] = "/400.html";
	getStatus[404] = "/404.html";
	getStatus[405] = "/405.html";
	getStatus[505] = "/505.html";

	this->_statusCode = code + str;
	if (status > 299)
		this->_url = getStatus[status];
}

void		HTTPResponse::setFileName(const std::string &file)
{
	_fileName = file;
}

std::string HTTPResponse::checkUrl()
{
	std::string filename("html");
	std::string tmpname("html");
	int fd;

	this->setStatus(this->_statusCode, "");
	tmpname += this->_url;
	if (this->_url == "/")
		this->_url = "/index.html";
	else if ((fd = open(tmpname.c_str(), O_RDWR)) == -1)
		this->setStatus("404", " Not Found");
	filename += this->_url;
	std::cout << "stat = "<< _statusCode << std::endl;
	close(fd);
	return filename;
}

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
//	std::cout << "stat = "<< _statusCode << "prot =" << _protocol << "url = " <<_url << std::endl;
}

void HTTPResponse::rendering( void )
{
//	std::cout << " redener stat = "<< _statusCode << "prot =" << _protocol << "url = " <<_url << std::endl;
	time_t rawtime;
	time(&rawtime);
	std::string	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
	this->_header += "Content-Type: text/html; charset=UTF-8\r\n";
	this->_header += "Referrer-Policy: no-referrer\r\n";
	this->_header += "Content-Length: " + this->_contentLen + "\r\n";
	this->_header += "Date: " + timeStr; 
	std::cout << this->_header << std::endl;
}

void HTTPResponse::rendering(const std::string typeContent)
{
	time_t rawtime;
	time(&rawtime);
	std::string	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
	this->_header += "Content-Type: " + typeContent + "\r\n";
	this->_header += "Referrer-Policy: no-referrer\r\n";
	this->_header += "Content-Length: " + this->_contentLen + "\r\n";
	this->_header += "Date: " + timeStr;
	std::cout << this->_header << std::endl;
}

void HTTPResponse::rendering(const std::string typeContent, bool b)
{
	time_t rawtime;
	time(&rawtime);
	(void)b;

	std::string	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
	this->_header += "Server: webserv\r\n"; // replace by server_name so we need to access the socket
	this->_header += "Date: " + timeStr + "\r\n";
	this->_header += "Content-Type: " + typeContent + "\r\n";
	this->_header += "Content-Length: " + this->_contentLen + "\r\n";
	this->_header += "Accept-Ranges: bytes";
	std::cout << this->_header << std::endl;
}
