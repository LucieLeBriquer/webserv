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

int		sendReponse(int fde, std::string header)
{
	//std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";

	if (send(fde, header.c_str(), header.size(), 0) < 0)
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
	HTTPRequest			treat;
	HTTPResponse		deliver;
	int					i;

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
			if (i == -1)
			{
				std::cout << "Connection closed by foreign host." << std::endl;
				break ;	
			}
			if (strcmp(&string[string.length() - 4], "\r\n\r\n") == 0)
				break ;
		}
		else
		{
			i = treat.method(buf, &deliver);
			if (treat.header(buf) == -1)
				std::cout << "HTTP/1.1 404 Not Found [continue]" << std::endl;
			deliver.header();
			recv_len += byteCount;
			std::cout << "get " << recv_len << " bytes from " << fde << " : buf = [ " << buf << " ] " << std::endl;
		}
		string += buf;
	}
	std::cout << "final string = " << string << std::endl;
	std::cout << "header = " << deliver.getHeader() << std::endl;
	if (sendReponse(fde, deliver.getHeader()) < 0)
		return -1;
	return 1;
}