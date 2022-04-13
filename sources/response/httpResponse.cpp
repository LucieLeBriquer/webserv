/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/12 15:15:56 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName(""), _location(""), _redir(0)
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

std::string HTTPResponse::checkUrl(std::string filename)
{
	int fd;

	if (this->_location != "")
		this->_url = this->_location;
	if ((fd = open(filename.c_str(), O_RDWR)) == -1)
		this->setStatus("404", " Not Found");
	else
		this->setStatus(this->_statusCode, "");
	close(fd);
	return filename;
}

std::string HTTPResponse::redirect(Socket &sock, int sockNbr)
{
//Verifier si la listen directive ne passe pas une requete à un autre serveur
//
	std::string filename;
	filename = sock.getRealUrl(sockNbr, this->_url);
//	filename = filename.substr(1, filename.length() - 1);
 std::cout << "filename = " << filename <<std::endl; 
 std::cout << "status = " << _statusCode <<std::endl; 

	if (sock.isRedir(sockNbr, filename))
	{
		this->_location = sock.getRedir(sockNbr, filename);
 std::cout << "location = " << _location <<std::endl; 
		this->_statusCode = "301 Moved Permanently";
	}
	this->checkUrl(filename);
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
	this->_header += header.fillrender();
	if (this->_redir)
		this->_header += "Location: " + this->_location + "\r\n";
	this->_header += "Content-Length: " + this->_contentLen + "\r\n";	
	this->_header += "Date: " + timeStr; 
	std::cout << this->_header << std::endl;
}

