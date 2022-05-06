/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/05/06 15:07:40 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

HTTPResponse::HTTPResponse(void) : _options(""), _contentLen(""), _protocol(""), _statusCode(""), _url(""),
									_header(""), _method(""), _fileName(""), _location(""), 
									_statusNb(0), _redir(0), _needAutoindex(false), _chunked(0), _max_size_c(0)
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

std::string	HTTPResponse::getProtocol(void) const
{
	return (_protocol);
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

size_t		HTTPResponse::getMaxSizeC(void) const
{
	return (_max_size_c);
}

bool		HTTPResponse::getNeedAutoindex(void) const
{
	return (_needAutoindex);
}

int			HTTPResponse::isChunked( void )
{
	if (this->_chunked)
		return (1);
	return (0);
}

int 		HTTPResponse::setStatus(std::string code, std::string str, HTTPHeader &header)
{
	std::stringstream ss;
	int status;
	
	ss << code;
	ss >> status;


	this->_statusCode = code + str;
	if (status > 299)
		header.setContentTypeResponse("text/html");
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

void	HTTPResponse::setMaxSizeC(size_t value)
{
	_max_size_c = value;
}

std::string HTTPResponse::redirect(Socket &sock, int sockNbr, HTTPHeader &header)
{
//Verifier si la listen directive ne passe pas une requete à un autre serveur
//
	std::string filename;
	filename = sock.getRealUrl(sockNbr, this->_url);

	if ( this->_method == "GET" )
	{
		if (this->_url.find('?') != std::string::npos )
		{
			sock.setEnvValue("QUERY_STRING", this->_url.substr(this->_url.find('?') + 1, this->_url.length()));	
			sock.setIsQueryString(true);
			sock.setBody( sock.getEnvValue("QUERY_STRING") );
			this->_url = this->_url.substr(0, this->_url.find('?'));
		}
	}
	if (this->_method == "DELETE" && sock.isAllowedMethod(sockNbr, this->_url, getMethodNb("DELETE")))
	{
		if (this->checkUrl(sock, sockNbr, header) != "")
		{
			std::remove(filename.c_str());
			this->setStatus("200", " OK", header);
			return this->_url = "html/deleted.html";
		}
		this->setStatus("204", " No Content", header);
		this->_url = "";
		return this->_url;
	}
	if (this->_method == "OPTIONS" && sock.isAllowedMethod(sockNbr, this->_url, getMethodNb("OPTIONS")))
	{
		vecInt	methods = sock.getAllowedMethods(sockNbr, this->_url);
		for (size_t i = 0; i < methods.size() ; i++)
		{
			this->_options += ::getMethod(i);
			if (i != (methods.size() - 1))
				this->_options += ", ";
		}
	}
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

	pageErr = sock.errorPage(sockNbr, _url, _statusNb);
	if (pageErr != "") // error page precised
	{
		std::cout << "pageErr = " << pageErr << std::endl;
		this->_location = pageErr;
		this->_statusCode = "302 Moved Temporarily";
		this->_redir = 1;
		this->_statusNb = 302;
		this->_contentLen = "154";
	}
	return ("");
}

std::string	HTTPResponse::_returnSetErrPage(Socket &sock, int sockNbr, std::string code,
											std::string str, HTTPHeader &header)
{
	std::string	pageErr;

	setStatus(code, str, header);
	return (_returnErrPage(sock, sockNbr));
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
	if (this->_method == "OPTIONS")
		this->_header += "Allow: " + this->_options + "\r\n";
	this->_header += header.fillrender();
	if (this->_redir)
        this->_header += "Location: " + this->_location + "\r\n";		
	this->_header += "Date: " + timeStr + "\r\n";
	if (header.isChunked())
	{
		this->_chunked = 1;
		this->_header += "Transfer-Encoding: chunked";
	}
	else if (this->_contentLen != "")
		this->_header += "Content-Length: " + this->_contentLen ;
}
// GET / HTTP/1.1
// Transfer-Encoding: chunked