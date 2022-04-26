/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 15:10:02 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/26 11:27:25 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <csignal>

int		endRequest(std::string string, Socket &sock)
{
	if (strncmp(string.c_str(), "GET", 3) == 0)
		sock.setMethod(GET);
	else if (strncmp(string.c_str(), "POST", 4) == 0)
		sock.setMethod(POST);
	else if (strncmp(string.c_str(), "HEAD", 4) == 0)
		sock.setMethod(HEAD);
	else if (strncmp(string.c_str(), "DELETE", 6) == 0)
		sock.setMethod(DELETE);
	else if (strncmp(string.c_str(), "OPTIONS", 7) == 0)
		sock.setMethod(OPTIONS);
	else
		sock.setMethod(BAD_METHODE);
	// if (sock.getMethod() == BAD_METHODE)
	// 	return (ERR);
	//std::cout << "methode = " << sock.getMethod() << std::endl;

	for (size_t i = 0; i < string.length(); i++)
	{
		if (strncmp(&string[i], "\r\n\r\n", 4) == 0)
		{
			if (sock.getMethod() != POST)
			{
				std::string tmp = "";
				sock.setBody(tmp);
				return (ERR);
			}
			for (size_t j = i + 4; j < string.length() && sock.getMethod() == POST; j++)
			{
				if (strncmp(&string[i], "\r\n", 2) == 0)
				{
					std::string tmp = &string[j];
					sock.setBody(tmp);
					return (ERR);
				}
			}
		}
	}
	return (OK);
}