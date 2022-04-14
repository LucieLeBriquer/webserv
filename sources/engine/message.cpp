/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/14 14:37:02 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static int	getRightFile(HTTPResponse &response, Socket &sock, int sockNbr, HTTPHeader &header)
{
	std::string 		filename;
	size_t				size;

	size = 0;
	filename = response.checkUrl(sock, sockNbr);
	if (filename == "")
		return (ERR);
	response.setFileName(filename);

	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();
	response.setContentLen(size);
	response.rendering(header);
	return (OK);
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

int		sendResponse(int fde, HTTPResponse &response, HTTPHeader &header, Socket &sock, int sockNbr)
{
	//check methode et file pour cgi ou non

	// check if method is allowed for the requested url
	if (!sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
	{
		std::cerr << response.getUrl() << " method = " << response.getMethod() << std::endl;
		response.setStatus("405", " Method Not Allowed");
	}

	// fill header
	if (getRightFile(response, sock, sockNbr, header))
	{
		// if even page and err page are unavailable, print a small page according to the statusNb
		return (OK);
	}

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
			std::cout << "\n====================================================" << std::endl << std::endl;
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