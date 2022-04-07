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

void	GetRightFile(HTTPResponse *deliver, std::string *file)
{
	std::string 		body;
	std::string 		filename;
	int			size;
	int			fd;
	int			ret;
	char		buf[B_SIZE + 1];

	size = 0;
	filename = deliver->checkUrl();
	fd = open(filename.c_str(), O_RDWR);
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		body += buf;
		size += ret;
	}
	deliver->setContentLen(size);
	deliver->rendering();
	*file += deliver->getHeader();
	(*file) += "\n\n";
	(*file) += body;
	std::cout << "body =\n" << body << std::endl;
	std::cout << "file =\n" << *file << std::endl;
}

int		sendReponse(int fde, HTTPResponse *deliver)
{
	std::string	file;

	GetRightFile(deliver, &file);
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

int		requestReponse(int epollfd, int fde)
{
	char		buf[BUFFER_SIZE] = {0};
	int			byteCount, recv_len = 0;
	std::string		string;
	HTTPRequest		treat;
	HTTPHeader		head;
	STATUS			code;
	HTTPResponse	deliver;
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
				deliver.statusCode(code.status(4, 0), head.getFirstLine());
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
	if (sendReponse(fde, &deliver) < 0)
		return -1;
	return 1;
}