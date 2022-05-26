/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 14:30:31 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/26 13:15:00 by lle-briq         ###   ########.fr       */
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

static int	readBlock(Client &client, std::stringstream &fileStream)
{
	std::string			block = "";
	char				c;
	char				toCompare[2];
	size_t				pos;

	pos = client.getReadPos();
	while (fileStream.get(c) && pos < client.getReadBlock() + client.getBlockSize() + 2)
	{
		toCompare[0] = toCompare[1];
		toCompare[1] = c;
		block += c;
		pos++;
	}
	client.setReadPos(pos);
 	if (client.isBlockEnd() && toCompare[0] ==  '\r' && toCompare[1] == '\n')
	{
		write(client.getFdTmp(), block.c_str(), client.getBlockSize());
		client.setRecvBlockSize(false);
	}
	if (!fileStream.eof())
		return (ERR);
	return (OK);
}

static int	manageChunkedBody(Client &client)
{
	std::string			request = client.getRequest();
	if (client.getReadPos() >= request.size())
		return (OK);
	
	char				toCompare[2];
	std::stringstream	fileStream(request.substr(client.getReadPos()), std::ios::in | std::ios::binary);
	char				c;
	std::string			size = "";
	size_t				pos = client.getReadPos();

	if (!client.hasRecvBlockSize())
	{
		while (fileStream.get(c) && isHexaChar(c))
		{
			size += c;
			pos++;
		}
		toCompare[0] = c;
		if (fileStream.eof())
			return (OK);
		fileStream.get(toCompare[1]);
		if (fileStream.eof())
			return (OK);
		
		if (toCompare[0] ==  '\r' && toCompare[1] == '\n')
		{
			if (size == "0")
				return (ERR);
			client.setRecvBlockSize(true);
			client.setBlockSize(hexToInt(size));
			pos += 2;
			client.setReadPos(pos);
			client.setReadBlock(pos);
		}
		else
		{
			client.getResponse().statusCode(client.getStatus().status(4, 0), client.getHeader().getFirstLine());
			return (ERR);
		}
	}
	else
	{
		if (readBlock(client, fileStream))
			return (manageChunkedBody(client));
	}
	return (manageChunkedBody(client));
}

static int	writeHeader(Client &client, int status)
{
	HTTPHeader	&header = client.getHeader();
	std::string	request = client.getRequest();

	if (!header.isChunkedEncoded())
		write(client.getFdTmp(), request.c_str(), client.getTotSize());
	return (status);
}

int		endRequest(Client &client)
{
	HTTPHeader	&header = client.getHeader();
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
				std::cout << GRAY << std::endl << "<----------- Request ----------->";
				std::cout << std::endl << request.substr(0, i + 2) << END << std::endl;
				client.setHeaderSize(i + 4);
				if (client.getMethod() == POST && client.getHeaderSize() == request.size())
				{
					if (checkHeader(header, request) || (header.getContentLenValue() == "" && !header.isChunkedEncoded()))
						return (writeHeader(client, ERR));
					return (writeHeader(client, OK));
				}
				if (checkHeader(header, request))
					return (writeHeader(client, ERR));
				writeHeader(client, OK);
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
			if (header.isChunkedEncoded())
				return (manageChunkedBody(client));
			else if (client.getBodySize() >= header.getContentLenSize())
				return (ERR);
		}
	}
	return (OK);
}
