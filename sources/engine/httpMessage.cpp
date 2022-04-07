/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/07 14:14:45 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void	GetRightFile(std::string *file, int *tot_size)
{
	std::string 		body, head;
	std::stringstream	ss;
	int					fd;
	int					ret, head_size, body_size;
	char				buf[B_SIZE + 1];

	// open le bon file en fonction de la requete
	// fd = open("./html/index.html", O_RDWR);
	fd = open("./html/form.html", O_RDWR);
	// fd = open("./html/info.php", O_RDWR);
	// remplir le header reponse
	head = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	// garder la size a jour pour send()
	head_size = head.length();
	// lire notre file open
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		body += buf;
		(*tot_size) += ret;
	}
	// recup le content length
	ss << (*tot_size);
	head += ss.str();
	// set les 2 \n avant le body
	head += "\n\n";
	// taille du content length et les 2 \n
	head_size += (ss.str()).length() + 2;
	// response entiere
	(*file) = head + body;
	(*tot_size) += head_size;
}

int		sendReponse(int fde, int flag, Socket *sock, int sockNbr)
{
	std::string	file;
	int			tot_size = 0;
	
	if (flag == 4)
		GetCGIfile(&file, &tot_size, sock->getEnv());
	else
		GetRightFile(&file, &tot_size);
	if (send(fde, file.c_str(), tot_size, 0) < 0)
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
	std::string	string;
	int			flag = 0, body = 0;

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
			/************************************************/
			if (strncmp(string.c_str(), "GET", 3) == 0)
				flag = 1;
			else if (strncmp(string.c_str(), "POST", 4) == 0)
				flag = 2;
			else if (strncmp(string.c_str(), "HEAD", 4) == 0)
				flag = 3;
			if (strncmp(&string[5], "/php-example.php", 16) == 0)
				flag = 4;
			/*************************************************/
			if (flag == 4)
				break ;
			if (strcmp(&string[string.length() - 4], "\r\n\r\n") == 0 && flag != 4)
				break ;
		}
		else
		{
			recv_len += byteCount;
			std::cout << "get " << recv_len << " bytes from " << fde << " : buf = [ " << buf << " ] " << std::endl;
		}
		string += buf;
	}
	std::cout << "final string = " << string << std::endl;
	if (sendReponse(fde, flag, sock, sockNbr) < 0)
		return -1;
	return 1;
}