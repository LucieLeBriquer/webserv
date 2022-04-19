/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/19 16:36:31 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName(""), _location(""), 
									_statusNb(0), _redir(0), _needAutoindex(false)
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
		_needAutoindex = response._needAutoindex;
	}
	return (*this);
}

std::string HTTPResponse::getMethod(void) const
{
	return this->_method;
}

std::string HTTPResponse::getContentLen(void) const
{
	return this->_contentLen;
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

int			HTTPResponse::getRedir(void) const
{
	return (_redir);
}

bool		HTTPResponse::getNeedAutoindex(void) const
{
	return (_needAutoindex);
}

int 		HTTPResponse::setStatus(std::string code, std::string str, HTTPHeader &header)
{
	std::stringstream ss;
	std::map<int, std::string> getStatus;
	int status;
	
	ss << code;
	ss >> status;

	getStatus[400] = "/400.html";
	getStatus[403] = "/403.html";
	getStatus[404] = "/404.html";
	getStatus[405] = "/405.html";
	getStatus[505] = "/505.html";

	this->_statusCode = code + str;
	if (status > 299)
	{
		this->_url = getStatus[status];
		header.setContentTypeResponse("text/html");
	}
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

void		HTTPResponse::setUrl(const std::string &url)
{
	_url = url;
}

void		HTTPResponse::setRedir(int r)
{
	_redir = r;
}

std::string HTTPResponse::redirect(Socket &sock, int sockNbr, HTTPHeader &header)
{
//Verifier si la listen directive ne passe pas une requete à un autre serveur
//
	std::string filename;
	filename = sock.getRealUrl(sockNbr, this->_url);

	if (sock.isRedir(sockNbr, this->_url))
	{
		this->_location = sock.getRedir(sockNbr, _url);
		this->_statusCode = "301 Moved Permanently";
		this->_redir = 1;
		this->_contentLen = "178";
		return "";
	}
	return (this->checkUrl(sock, sockNbr, header));
}

std::string	HTTPResponse::_returnErrPage(Socket &sock, int sockNbr)
{
	std::string	pageErr;
	int			fd;

	pageErr = sock.errorPage(sockNbr, _url, _statusNb);
	if ((fd = open(pageErr.c_str(), O_RDWR)) == -1)
		return ("");
	close(fd);
	return (pageErr);
}

std::string	HTTPResponse::_returnSetErrPage(Socket &sock, int sockNbr, std::string code,
											std::string str, HTTPHeader &header)
{
	std::string	pageErr;
	int			fd;

	setStatus(code, str, header);
	pageErr = sock.errorPage(sockNbr, _url, _statusNb);
	if ((fd = open(pageErr.c_str(), O_RDWR)) == -1)
		return ("");
	close(fd);
	return (pageErr);
}

std::string	HTTPResponse::_manageDirectory(Socket &sock, int sockNbr, HTTPHeader &header)
{
	int			fd;
	vecStr		index;
	std::string	indexStr;

	if (sock.isRootPath(sockNbr, _url))
	{
		index = sock.getIndex(sockNbr, _url);
		for (size_t i = 0; i < index.size(); i++)
		{
			indexStr = sock.addRoot(sockNbr, _url, index[i]);
			std::cout << indexStr << std::endl;
			if ((fd = open(indexStr.c_str(), O_RDWR)) != -1)
			{
				close(fd);
				return (indexStr);
			}
		}
	}
	if (!sock.getAutoindex(sockNbr, _url))
		return (_returnSetErrPage(sock, sockNbr, "403", " Forbiden", header));
	_needAutoindex = true;
	return ("");
}

std::string HTTPResponse::checkUrl(Socket &sock, int sockNbr, HTTPHeader &header)
{
	std::string filename;
	std::string	pageErr;
	int			fd;

	// check if there was an error before (method not allowed etc)
	if (_statusNb != 0)
		return (_returnErrPage(sock, sockNbr));

	filename = sock.getRealUrl(sockNbr, _url);
	std::cout << PURPLE << "[Trying to get]" << END ;
	if (isDirectory(filename))
		std::cout << " directory ";
	else
		std::cout << " file ";
	std::cout << PURPLE << "\"" << filename << "\"" << END << std::endl << std::endl;

	// if it's a directory
	if (isDirectory(filename))
		return (_manageDirectory(sock, sockNbr, header));

	// if it's a file
	if ((fd = open(filename.c_str(), O_RDWR)) == -1)
		return (_returnSetErrPage(sock, sockNbr, "404", " Not Found", header));
	
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
	if (this->_contentLen != "")
		this->_header += "Content-Length: " + this->_contentLen + "\r\n";
	this->_header += "Date: " + timeStr;
}
