/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/12 10:18:38 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName(""), _location("")
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
	return (*this);
}

std::string HTTPResponse::getMethod( void )
{
	return this->_method;
}

std::string HTTPResponse::getStatus( void )
{
	return this->_statusCode;
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

int HTTPResponse::setStatus(std::string code, std::string str)
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
	return status;
}

void		HTTPResponse::setFileName(const std::string &file)
{
	_fileName = file;
}

void HTTPResponse::redirect(Socket *sock, int sockNbr)
{
//Verifier si la listen directive ne passe pas une requete à un autre serveur
//

//	sock->getConfig(sockNbr).getHost
(void)sock;
(void)sockNbr;
	this->_location = "index.html";
}

std::string HTTPResponse::checkUrl(Socket *sock, int sockNbr)
{
	std::string filename("html");
	std::string tmpname("html");
	int fd;

	this->_location = "";
	int status = this->setStatus(this->_statusCode, "");
	tmpname += this->_url;
	if (this->_url == "/")
		this->_url = "/index.html";
	else if ((fd = open(tmpname.c_str(), O_RDWR)) == -1)
		this->setStatus("404", " Not Found");
	filename += this->_url;
	if (status == 200)
		this->redirect(sock, sockNbr);
	close(fd);
	return filename;
}

void	HTTPResponse::setContentLen(int len)
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
}

void HTTPResponse::rendering( HTTPHeader &header )
{
	time_t rawtime;
	time(&rawtime);
	std::string	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
	this->_header += "Content-Type: text/html; charset=UTF-8\r\n";
	this->_header += header.fillrender();
	this->_header += "Content-Length: " + this->_contentLen + "\r\n";	
	this->_header += "Date: " + timeStr; 
	std::cout << this->_header << std::endl;
}

// void HTTPResponse::rendering(const std::string typeContent, HTTPHeader &header)
// {
// 	time_t rawtime;
// 	time(&rawtime);
// 	std::string	timeStr = ctime(&rawtime);
// 	timeStr = timeStr.substr(0, timeStr.size() - 1);
// 	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
// 	this->_header += "Content-Type: " + typeContent + "\r\n";
// 	this->_header += header.fillrender();
// 	// this->_header += "Referrer-Policy: no-referrer\r\n";
// 	this->_header += "Content-Length: " + this->_contentLen + "\r\n";
// 	this->_header += "Date: " + timeStr;
// 	std::cout << this->_header << std::endl;
// }

// void HTTPResponse::rendering(const std::string typeContent, HTTPHeader &header, bool b)
// {
// 	time_t rawtime;
// 	time(&rawtime);
// 	(void)b;
// 	(void)header;

// 	std::string	timeStr = ctime(&rawtime);
// 	timeStr = timeStr.substr(0, timeStr.size() - 1);
// 	this->_header = this->_protocol + ' ' + this->_statusCode + "\r\n";
// 	this->_header += "Server: webserv\r\n"; // replace by server_name so we need to access the socket
// 	this->_header += "Date: " + timeStr + "\r\n";
// 	this->_header += "Content-Type: " + typeContent + "\r\n";
// 	this->_header += "Content-Length: " + this->_contentLen + "\r\n";
// 	this->_header += "Accept-Ranges: bytes";
// 	std::cout << this->_header << std::endl;
// }

