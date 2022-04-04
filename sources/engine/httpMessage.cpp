/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_reponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 13:44:54 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include "usefull.hpp"

int		requestReponse(int epollfd, int fde)
{
	char		buf[30000] = {0};
	int			byteCount;
	HTTPRequest			treat;
	HTTPResponse		deliver;

	memset(buf, 0, sizeof(buf));
	while (1)
	{
		byteCount = recv(fde, buf, sizeof(buf), MSG_DONTWAIT);
		if (byteCount == 0)
		{
			epoll_ctl(epollfd, EPOLL_CTL_DEL, fde, NULL);
			break ;
		}
		else if (byteCount < 0)
		{
			close(fde);
			break ;
		}
		else
		{
			std::cout << buf << std::endl;
			int i = 0;
			i = treat.method(buf, &deliver);
			if (treat.header(buf) == -1)
			{
				std::cout << "HTTP/1.1 404 Not Found [continue]" << std::endl;
			}
			deliver.header();
			if (send(fde, (deliver.getHeader()).c_str() , (deliver.getHeader()).length(), MSG_DONTWAIT) < 0)
			{
				perror("send()");
				return -1;
			}
			std::cout << deliver.getHeader() << std::endl;
//			std::cout << "get " << byteCount << " bytes of content from " << fde << " [ " << buf << " ] " << std::endl;
			std::cout << "sending data to " << fde << std::endl;
			if (i == -1)
			{
				//en fonction du user-agent : curl ou telnet ou mozilla  (curl/7.68.0)
				std::cout << "Connection closed by foreign host." << std::endl;
				exit(EXIT_FAILURE);
			}
			close(fde);
		}
	}
	return 1;
}