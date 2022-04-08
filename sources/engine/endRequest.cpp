/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 15:10:02 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/08 15:11:47 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int		endRequest(std::string string, Socket *sock)
{
	if (strncmp(string.c_str(), "GET", 3) == 0)
		sock->setMethode(GET);
	else if (strncmp(string.c_str(), "POST", 4) == 0)
		sock->setMethode(POST);
	else
		sock->setMethode(BAD_METHODE);
	std::cout << "methode = " << sock->getMethode() << std::endl;
	for (int i = 0; i < string.length(); i++)
	{
		if (strncmp(&string[i], "\r\n\r\n", 4) == 0)
		{
			if (sock->getMethode() == GET || sock->getMethode() == BAD_METHODE)
				return 1;
			for (int j = i + 4; j < string.length() && sock->getMethode() == POST; j++)
			{
				if (strncmp(&string[i], "\r\n", 2) == 0)
					return 1;
			}
		}
	}
	return 0;
}