/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/14 14:17:15 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName(""), _location(""), 
									_statusNb(0), _redir(0)
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
		_location = response._location;
		_statusNb = response._statusNb;
		_redir = response._redir;
	}
	return (*this);
}

std::string HTTPResponse::getMethod(void) const
{
	return this->_method;
}

std::string HTTPResponse::getStatus(void) const
{
	return this->_statusCode;
}

std::string HTTPResponse::getUrl(void) const
{
	return this->_url;
}

std::string HTTPResponse::getHeader(void) const
{
	return this->_header;
}

std::string	HTTPResponse::getFileName(void) const
{
	return (_fileName);
}

int			HTTPResponse::getStatusNb(void) const
{
	return (_statusNb);
}

int			HTTPResponse::getMethodNbr(void) const
{
	return (getMethodNb(this->getMethod()));
}

int 		HTTPResponse::setStatus(std::string code, std::string str)
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
	setStatusNb(status);
	return status;
}

void 		HTTPResponse::setStatusNb(int nb)
{
	_statusNb = nb;
}

void		HTTPResponse::setFileName(const std::string &file)
{
	_fileName = file;
}

void		HTTPResponse::setMethod(const std::string &method)
{
	_method = method;
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
	//_redir=1;
	
	//_location = "/index.html";
	//_statusCode = "301 Moved Permanently";

	return (sock.getRealUrl(sockNbr, filename));
}

std::string HTTPResponse::checkUrl(Socket &sock, int sockNbr)
{
	std::string filename;
	std::string	pageErr;
	int			fd;

	// what's the point of this ? redir ?
	/*if (_location != "")
		_url = _location;
	setStatus(_statusCode, "");*/

	// check if there was an error before (method not allowed etc)
	if (_statusNb != 0)
	{
		pageErr = sock.errorPage(sockNbr, _url, _statusNb);
		if ((fd = open(pageErr.c_str(), O_RDWR)) == -1)
			return ("");
		close(fd);
		return (pageErr);
	}

	filename = sock.getRealUrl(sockNbr, _url);
	if ((fd = open(filename.c_str(), O_RDWR)) == -1)
	{
		setStatus("404", " Not Found");
		pageErr = sock.errorPage(sockNbr, _url, 404);
		if ((fd = open(pageErr.c_str(), O_RDWR)) == -1)
			return ("");
		close(fd);
		return (pageErr);
	}
	
	//filename = redirect(sock, sockNbr, _url);
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
