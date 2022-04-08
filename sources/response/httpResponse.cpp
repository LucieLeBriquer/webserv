/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:41:57 by masboula          #+#    #+#             */
/*   Updated: 2022/04/08 17:04:33 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

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
//	std::cout << "stat = "<< _statusCode << std::endl;
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
	if (this->_location != "")
		this->_header = "\n"+ this->_protocol + ' ' + "301" + "\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nLocation: "+ this->_location +"\nDate: " + ctime(&rawtime); 
	else
		this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode +  "\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nDate: " + ctime(&rawtime); 
	std::cout << this->_header << std::endl;
}

// added for CSS
void HTTPResponse::rendering(const std::string typeContent)
{
	time_t rawtime;
	time(&rawtime);
	if (this->_location != "")
		this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: " + typeContent + "\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nLocation: "+ this->_location +"\nDate: " + ctime(&rawtime); 
	else
		this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: " + typeContent + "\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nDate: " + ctime(&rawtime); 
	std::cout << this->_header << std::endl;
}
//sock->getconfg()
// 