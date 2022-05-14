/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 14:30:31 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/14 14:30:31 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void	isDownloading(HTTPHeader &header, HTTPResponse &response)
{
	std::string	hdr = response.getHeader();
	std::string filename;

	if (strncmp(response.getUrl().c_str(), "/download/", 10) == 0)
	{
		filename = &(response.getUrl().c_str())[10];
		if (strcmp(header.getSecFetchDest().c_str(), "document") == 0 && strcmp(header.getSecFetchMode().c_str(), "navigate") == 0)
		{
			hdr += "\r\nContent-Disposition: attachment; filename=\"" + filename + "\"";
			hdr += "\r\nContent-Type: application/octet-stream";
			hdr += "\r\nContent-Description: File Transfer";
			response.setHeader(hdr);
		}
	}
}

int		endRequest(Client &client)
{
	FILE		*file = client.getTmp();
	int 		lastChar[4];

	if (client.getMethod() == BAD_METHOD)
	{
		for (int i = 0; i < nbMethods; i++)
		{
			if (strncmp(client.getRequest().c_str(), methods[i].c_str(), methods[i].size()) == 0)
				client.setMethod(getMethodNb(methods[i]));
		}
	}
	
	if (!client.hasRecvHeader())
	{
		std::rewind(file);
		while (!std::feof(file))
		{
			std::fseek(file, -3, SEEK_CUR);
			lastChar[0] = std::fgetc(file);
			lastChar[1] = std::fgetc(file);
			lastChar[2] = std::fgetc(file);
			lastChar[3] = std::fgetc(file);

			if (lastChar[0] == '\r' && lastChar[1] == '\n'
				&& lastChar[2] == '\r' && lastChar[3] == '\n')
			{
				client.changeRecvHeader();
				std::fgetc(file);
				if (client.getMethod() == POST && std::feof(file))
				{
					std::fseek(file, -1, SEEK_CUR);
					return (OK);
				}
				std::fseek(file, -1, SEEK_CUR);
				break;
			}
		}
	}

	if (client.hasRecvHeader())
	{
		if (client.getMethod() != POST)
			return (ERR);
		else
		{
			std::fseek(file, -2, SEEK_END);
			lastChar[0] = std::fgetc(file);
			lastChar[1] = std::fgetc(file);
			if (lastChar[0] == '\r' && lastChar[1] == '\n')
				return (ERR);
		}
	}
	std::fseek(file, 0, SEEK_END);
	return (OK);
}
