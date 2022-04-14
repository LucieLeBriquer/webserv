/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/14 08:37:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

bool	isPngFile(std::string name)
{
	if (name.size() < 4)
		return (false);
	if (strcmp(name.substr(name.size() - 4, 4).c_str(), ".png") == 0)
		return (true);
	return (false);	
}

bool	isCssFile(std::string name)
{
	if (name.size() < 4)
		return (false);
	if (strcmp(name.substr(name.size() - 4, 4).c_str(), ".css") == 0)
		return (true);
	return (false);	
}

static void	getRightFile(HTTPResponse &response, Socket &sock, int sockNbr, HTTPHeader &header)
{
	std::string 		filename;
	size_t				size;

	size = 0;
	filename = response.checkUrl(sock, sockNbr);
	response.setFileName(filename);

	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();
	response.setContentLen(size);
	response.rendering(header);
}

static int	sendHeader(int fde, HTTPResponse &response)
{
	std::string	header = response.getHeader() + "\r\n\r\n";

	std::cout << "====================================================" << std::endl;
	std::cout << header;
	std::cout << "====================================================" << std::endl;
	if (send(fde, header.c_str(), header.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}

static int	sendData(int fde, HTTPResponse &response)
{
	std::ifstream	fileStream(response.getFileName().c_str(), std::ios::in | std::ios::binary);
	char			buf[BUFFER_SIZE];
	int				i;
	char			c;
	
	while (fileStream.get(c))
	{
		memset(buf, 0, BUFFER_SIZE);
		buf[0] = c;
		i = 1;
		while (fileStream.get(c) && i + 1 < BUFFER_SIZE)
		{
			buf[i] = c;
			i++;
		}
		if (i + 1 == BUFFER_SIZE)
		{
			buf[i] = c;
			i++;
		}
		if (send(fde, buf, i, 0) < 0)
		{
			perror("send()");
			fileStream.close();
			return (ERR);
		}
	}
	fileStream.close();
	return (OK);
}

int		sendResponse(int fde, HTTPResponse &response, HTTPHeader &header, Socket &sock, int sockNbr) // give sock and sockNbr to treat files
{
	//check methode et file pour cgi ou non

	// fill header
	getRightFile(response, sock, sockNbr, header);
	//(void)header;	// pour l'instant le parsing ne se fait pas mais quand on aura les données on pourra les fill dans le header de la réponse
					// ça sera beaucoup plus clean par exemple pour le type de fichier renvoyé

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

	// deliver header
	if (sendHeader(fde, response))
		return (ERR);

	// deliver data
	if (sendData(fde, response))
		return (ERR);
	
	// si code erreur (bad request ou autre) -> close(fde), si code succes on ne close pas le fd
	// std::cout << "status ="<<response.getStatus()<<std::endl;
	// if ((response.getStatus()).find("400") != std::string::npos )
		// close(fde);
	return (OK);
}

int		checkHeader(HTTPHeader &header, std::string string)
{
	string.erase(0, (getHead(string)).length() + 2);
	while (1)
	{
		if (string == "")
			break ;
		if (header.fillheader(&string) == -1)
			break ; // a changer en fonction du retour d'err
	}
	if (header.header() == -1)
		return ERR;
	return 1;
}

int		requestReponse(int epollfd, int fde, Socket *sock, int sockNbr)
{
	char			buf[BUFFER_SIZE] = {0};
	int				byteCount, recv_len = 0;
	std::string		string;
	HTTPRequest		request;
	HTTPResponse	response;
	HTTPHeader		header;
	Status			status;
	int				line(0), isBreak = 0;

	while (1)
	{
		memset(buf, 0, BUFFER_SIZE);
		byteCount = recv(fde, buf, BUFFER_SIZE, 0);
		if (byteCount == 0)
		{
			epoll_ctl(epollfd, EPOLL_CTL_DEL, fde, NULL);
			isBreak = 1;
			break ;
		}
		else if (byteCount < 0)
		{
			if (line == 0)
			{
				if (header.method(string, &status, &response) == -1)
					break ;
			}
			if (endRequest(string, *sock))
				break ;
			line++;
		}
		else
		{
			recv_len += byteCount;
			std::cout << GREEN << "[Received] " << END << recv_len << " bytes from " << fde << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << buf ;
			std::cout << "====================================================" << std::endl << std::endl;
		}
		string += buf;
	}
	if (isBreak == 0)
	{
		if (checkHeader(header, string) == -1)
			status.statusCode(status.status(4, 0), header.getFirstLine());
		if (sock->isCgi(sockNbr, response.getUrl()))
		{
			if (GetCGIfile(*sock, sockNbr) < 0)
				return ERR;
		}
		else
		{
			if (sendResponse(fde, response, header, *sock, sockNbr))
				return (ERR);
		}
	}
	return (OK);
}