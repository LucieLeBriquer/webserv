/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/08 14:45:01 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#define B_SIZE 2

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

void	GetRightFile(HTTPResponse *deliver, std::string *file, Socket *sock, int sockNbr)
{
	std::string 		body;
	std::string 		filename;
	int			size;
	int			fd;
	int			ret;
	char		buf[B_SIZE + 1];

	size = 0;
	filename = deliver->checkUrl(sock, sockNbr);
	fd = open(filename.c_str(), O_RDWR);
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		body += buf;
		size += ret;
	}
	deliver->setContentLen(size);

	// added css
	if (isCssFile(filename))
		deliver->rendering("text/css");
	else if (isPngFile(filename))
		deliver->rendering("image/png"); // not sufficient
	else
		deliver->rendering();
	*file += deliver->getHeader();
	(*file) += "\n\n";
	(*file) += body;
}

int		sendReponse(int fde, HTTPResponse *deliver,Socket *sock, int sockNbr)
{
	std::string	file;

	//check methode et file pour cgi ou non
	GetRightFile(deliver, &file, sock, sockNbr);
	if (send(fde, file.c_str(), file.length(), 0) < 0)
	{
		perror("send()");
		return -1;
	}
	std::cout << "sending data to " << fde << std::endl;
	/*si code erreur (bad request ou autre) -> close(fde), si code succes on ne close pas le fd*/
	close(fde);
	return 1;
}

int		requestReponse(int epollfd, int fde, Socket *sock, int sockNbr)
{
	char		buf[BUFFER_SIZE] = {0};
	int			byteCount, recv_len = 0;
	std::string		string;
	HTTPRequest		treat;
	HTTPResponse	deliver;
	HTTPHeader		head;
	STATUS			code;
	int				line;
	(void)sock;
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
			//req = string;
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
			if (strcmp(&string[string.length() - 4], "\r\n\r\n") == 0)
				break ;
			//req.clear();
			line++;
		}
		else
		{
			recv_len += byteCount;
			std::cout << "get " << recv_len << " bytes from " << fde << " : buf = [ " << buf << " ] " << std::endl;
		}
		string += buf;
	}
	if (sendReponse(fde, &deliver, sock, sockNbr) < 0)
		return -1;
	return 1;
}