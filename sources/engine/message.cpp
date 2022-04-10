/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/10 10:31:01 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static bool	isPngFile(std::string name)
{
	if (strcmp(name.substr(name.size() - 4, 4).c_str(), ".png") == 0)
		return (true);
	return (false);	
}

static bool	isCssFile(std::string name)
{
	if (strcmp(name.substr(name.size() - 4, 4).c_str(), ".css") == 0)
		return (true);
	return (false);	
}

static void	getRightFile(HTTPResponse &response)
{
	std::string 		filename;
	size_t				size;

	size = 0;
	filename = response.checkUrl();
	response.setFileName(filename);

	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();

	response.setContentLen(size);
	if (isCssFile(filename))
		response.rendering("text/css");
	else if (isPngFile(filename))
		response.rendering("image/avif");
	else
		response.rendering();
}

static int	sendHeader(int fde, HTTPResponse &response)
{
	std::string	header = response.getHeader() + "\r\n\r\n";

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

int		sendReponse(int fde, HTTPResponse &response, HTTPHeader &header) // give sock and sockNbr to treat files
{
	//check methode et file pour cgi ou non

	// fill header
	getRightFile(response);
	(void)header;	// pour l'instant le parsing ne se fait pas mais quand on aura les données on pourra les fill dans le header de la réponse
					// ça sera beaucoup plus clean par exemple pour le type de fichier renvoyé

	std::cout << "url = " << response.getUrl() << std::endl;
	std::cout << "sending data to " << fde << std::endl;

	// deliver header
	if (sendHeader(fde, response))
		return (ERR);

	// deliver data
	if (sendData(fde, response))
		return (ERR);
	
	// si code erreur (bad request ou autre) -> close(fde), si code succes on ne close pas le fd
	close(fde);
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
	int				line;
	(void)sockNbr;

	line = 0;
	while (1)
	{
		memset(buf, 0, BUFFER_SIZE);
		byteCount = recv(fde, buf, BUFFER_SIZE, 0);
		if (byteCount == 0)
		{
			epoll_ctl(epollfd, EPOLL_CTL_DEL, fde, NULL);
			break ;
		}
		else if (byteCount < 0)
		{
			if (line == 0)
			{
				if (header.method(string, &status, &response) == -1)
				{
					std::cout << "Connection closed by foreign host." << std::endl;
					break ;
				}
			}
			else if (!header.header(string))
				status.statusCode(status.status(4, 0), header.getFirstLine());
			if (endRequest(string, sock))
				break ;
			line++;
		}
		else
		{
			recv_len += byteCount;
			std::cout << "get " << recv_len << " bytes from " << fde << " : buf = [ " << buf << " ] " << std::endl;
		}
		string += buf;
	}
	if (sendReponse(fde, response, header) < 0)
		return -1;
	return 1;
}