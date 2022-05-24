/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 14:30:31 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/24 13:53:57 by masboula         ###   ########.fr       */
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
	std::string	request = client.getRequest();
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
				if (client.getMethod() == POST && i + 4 == request.size())
				{
					if (checkHeader(client.getHeader(), request) || client.getHeader().getContentLenValue() == "")
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
			if (client.getBodySize() >= client.getHeader().getContentLenSize())
				return (ERR);
			if (client.getTotSize() <= client.getHeaderSize())
				return (OK);
		}
	}
	return (OK);
}
