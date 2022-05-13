/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 15:10:02 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/13 14:15:42 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

size_t	getFdSize(int fd)
{
	struct stat	info;
	
	fstat(fd, &info);
	return (info.st_size);
}

int		endRequest(FILE *file, Socket &sock, std::string string)
{
	int lastChar[4];

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

	if (sock.getMethod() == POST)
	{
		std::fseek(file, -2, SEEK_END);
		lastChar[0] = std::fgetc(file);
		lastChar[1] = std::fgetc(file);
		if (lastChar[0] == '\r' && lastChar[1] == '\n')
			return (ERR);
	}
	else
	{
		std::fseek(file, -4, SEEK_END);
		lastChar[0] = std::fgetc(file);
		lastChar[1] = std::fgetc(file);
		lastChar[2] = std::fgetc(file);
		lastChar[3] = std::fgetc(file);
		if (lastChar[0] == '\r' && lastChar[1] == '\n'
			&& lastChar[2] == '\r' && lastChar[3] == '\n')
			return (ERR);
	}
	std::fseek(file, 0, SEEK_END);
	return (OK);
}
