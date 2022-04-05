/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_reponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/01 10:29:48 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

#define B_SIZE 2

void	GetRightFile(HTTPResponse *deliver, int *tot_size, std::string *file)
{
	std::string 		body;
	int			fd;
	int			ret;
	char		buf[B_SIZE + 1];

	// open le bon file en fonction de la requete
	fd = open("html/index.html", O_RDWR);
	// lire notre file open
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		body += buf;
		(*tot_size) += ret;
	}
	// recup le content length
	std::cout << "laa" << *tot_size << std::endl;
	deliver->setContentLen(*tot_size);
	// remplir le header reponse
	deliver->rendering();
	*file += deliver->getHeader();
	// garder la size a jour pour send()
	(*tot_size) += (*file).length();
	std::cout << "laa aussi " << *tot_size << std::endl;

	// set les 2 \n avant le body
	(*file) += "\n\n";
	// taille du content length et les 2 \n
	(*tot_size) += 2;
	// response entiere
	(*file) += body;
}

int		sendReponse(int fde, HTTPResponse *deliver)
{
	std::string	file;
	int			tot_size = 0;

	GetRightFile(deliver, &tot_size, &file);
	if (send(fde, file.c_str(), tot_size, 0) < 0)
	{
		perror("send()");
		return -1;
	}
	std::cout << "sending data to " << fde << std::endl;
	/*si code erreur (bad request ou autre) -> close(fde), si code succes on ne close pas le fd*/
	close(fde);
	std::cout << "mais donc " << std::endl;
	return 1;
}

int		requestReponse(int epollfd, int fde)
{
	char		buf[BUFFER_SIZE] = {0};
	int			byteCount, recv_len = 0;
	std::string		string;
	HTTPRequest		treat;
	HTTPResponse	deliver;
	HTTPHeader		head;
	HTTPMethod		method;
	STATUS			code;
	int				i;
	int				line;

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
				if ((treat.method(string, &method, &code)) == -1)
				{
					std::cout << "Connection closed by foreign host." << std::endl;
					break ;
				}
			}
			else
			{
				if (!treat.header(string, &head))
					code.statusCode(code.status(4, 0), method.getProtocol());
			}
			if (strcmp(&string[string.length() - 4], "\r\n\r\n") == 0)
				break ;
			string.clear();
			line++;
		}
		else
		{
			recv_len += byteCount;
			std::cout << "get " << recv_len << " bytes from " << fde << " : buf = [ " << buf << " ] " << std::endl;
		}
		string += buf;
	}
//	std::cout << "final string = " << string << std::endl;
	if (sendReponse(fde, &deliver) < 0)
		return -1;
	std::cout << "mais donc la" << std::endl;
	return 1;
}