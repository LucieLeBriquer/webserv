/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_reponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/03/31 18:12:44 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int		requestReponse(int epollfd, int fde)
{
	char		buf[2] = {0};
	int			byteCount;
	// std::string string;

	memset(buf, 0, sizeof(buf));
	while (1)
	{
		byteCount = recv(fde, buf, sizeof(buf), MSG_DONTWAIT);
		// string += buf;
		// std::cout << "byteCounte = " << byteCount << "  string = " << string << std::endl;
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
			std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n";

			std::cout << "get " << byteCount << " bytes of content from " << fde << " [ " << buf << " ] " << std::endl;
			if (send(fde, hello.c_str(), hello.size(), MSG_DONTWAIT) < 0)
			{
				perror("send()");
				return -1;
			}
			std::cout << "sending data to " << fde << std::endl;
			close(fde);
		}
	}
	return 1;
}