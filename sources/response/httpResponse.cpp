/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/14 11:07:22 by lle-briq         ###   ########.fr       */
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

std::string HTTPResponse::redirect(Socket &sock, int sockNbr, std::string filename)
{
//Verifier si la listen directive ne passe pas une requete à un autre serveur
//
	std::string file;

	std::cout << "filename = " << filename <<std::endl; 
	std::cout << "real url = " << sock.getRealUrl(sockNbr, filename) <<std::endl;
	std::cout << "host = " << sock.getConfig(sockNbr).getHost() <<std::endl;

	//(void)sock;
	//this->_redir=1;
	
	//this->_location = "/index.html";
	//this->_statusCode = "301 Moved Permanently";

	return sock.getRealUrl(sockNbr, filename);
}

std::string HTTPResponse::checkUrl(Socket &sock, int sockNbr)
{
	std::string filename;
	int fd;

	if (this->_location != "")
		this->_url = this->_location;
	this->setStatus(this->_statusCode, "");

	filename = sock.getRealUrl(sockNbr, this->_url);
	filename = filename.substr(1, filename.size() - 1);
	// if (this->_url == "/")
	// 	this->_url = "/index.html";
	if ((fd = open(filename.c_str(), O_RDWR)) == -1)
		this->setStatus("404", " Not Found");
	
	//filename = this->redirect(sock, sockNbr, this->_url);
	close(fd);
	return (filename);
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
}
