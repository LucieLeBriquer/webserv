/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_reponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/05 10:45:46 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <sstream>
#define B_SIZE 2

void	GetRightFile(std::string *file, int *tot_size)
{
	std::string 		body;
	std::stringstream	ss;
	int			fd;
	int			ret;
	char		buf[B_SIZE + 1];

	// open le bon file en fonction de la requete
	fd = open("index.html", O_RDWR);
	// remplir le header reponse
	(*file) += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	// garder la size a jour pour send()
	(*tot_size) += (*file).length();
	// lire notre file open
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		body += buf;
		(*tot_size) += ret;
	}
	// recup le content length
	ss << (*tot_size);
	(*file) += ss.str();
	// set les 2 \n avant le body
	(*file) += "\n\n";
	// taille du content length et les 2 \n
	(*tot_size) += (ss.str()).length() + 2;
	// response entiere
	(*file) += body;
}

int		sendReponse(int fde)
{
	std::string	file;
	int			tot_size = 0;
	
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

int		requestReponse(int epollfd, int fde)
{
	char		buf[BUFFER_SIZE] = {0};
	int			byteCount, recv_len = 0;
	std::string	string;

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
			/*verifier chaque ligne si il s'agit d'une requete valide, si non, on break et bad request
			if (badRequest(string))
				break ; 
			*/
			if (strcmp(&string[string.length() - 4], "\r\n\r\n") == 0)
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
	if (sendReponse(fde) < 0)
		return -1;
	return 1;
}