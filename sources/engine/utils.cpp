/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 14:30:31 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/19 11:49:34 by lle-briq         ###   ########.fr       */
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
	std::string	request = client.getRequest();
	int 		lastChar[2];
	char		toCompare[4];
	
	if (!client.hasRecvHeader())
	{
		for (size_t i = 0; i + 3 < request.size(); i++)
		{
			for (int j = 0; j < 4; j++)
				toCompare[j] = request[i + j];
			if (toCompare[0] ==  '\r' && toCompare[1] == '\n'
				&& toCompare[2] == '\r' && toCompare[3] == '\n')
			{
				client.setHeaderSize(i + 4);
				std::cerr << "hop we got header" << std::endl;
				if (client.getMethod() == POST && i + 4 == request.size())
				{
					if (checkHeader(client.getHeader(), request))
						return (ERR);
					return (OK);
				}
				if (checkHeader(client.getHeader(), request))
					return (ERR);
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
			if (ftell(file) <= (long int)client.getHeaderSize())
				return (OK);
			lastChar[0] = std::fgetc(file);
			lastChar[1] = std::fgetc(file);
			if (lastChar[0] == '\r' && lastChar[1] == '\n')
				return (ERR);
		}
	}
	std::fseek(file, 0, SEEK_END);
	return (OK);
}
