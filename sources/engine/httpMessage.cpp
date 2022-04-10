/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/08 15:39:52 by lpascrea         ###   ########.fr       */
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

static void	getRightFile(HTTPResponse &deliver)
{
	std::string 		filename;
	size_t				size;

	size = 0;
	filename = deliver.checkUrl();
	deliver.setFileName(filename);

	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();

	deliver.setContentLen(size);
	if (isCssFile(filename))
		deliver.rendering("text/css");
	else if (isPngFile(filename))
		deliver.rendering("image/png");
	else
		deliver.rendering();
}

static int	sendHeader(int fde, HTTPResponse &deliver)
{
	std::string	header = deliver.getHeader() + "\r\n\r\n";

	if (send(fde, header.c_str(), header.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}

static int	sendData(int fde, HTTPResponse &deliver)
{
	std::ifstream	fileStream(deliver.getFileName().c_str(), std::ios::in | std::ios::binary);
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

int		sendReponse(int fde, HTTPResponse &deliver) // give sock and sockNbr to treat files
{
	//check methode et file pour cgi ou non

	// fill header
	getRightFile(deliver);

	std::cout << "url = " << deliver.getUrl() << std::endl;
	std::cout << "sending data to " << fde << std::endl;

	// deliver header
	if (sendHeader(fde, deliver))
		return (ERR);

	// deliver data
	if (sendData(fde, deliver))
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
	HTTPRequest		treat;
	HTTPResponse	deliver;
	HTTPHeader		head;
	STATUS			code;
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
				if (head.method(string, &code, &deliver) == -1)
				{
					std::cout << "Connection closed by foreign host." << std::endl;
					break ;
				}
			}
			else if (!head.header(string))
				code.statusCode(code.status(4, 0), head.getFirstLine());
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
	if (sendReponse(fde, deliver) < 0)
		return -1;
	return 1;
}