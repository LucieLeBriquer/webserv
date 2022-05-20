/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/20 16:23:01 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static int	sendHeader(int fde, Client &client, Socket &sock, bool redir, int sockNbr)
{
	HTTPResponse	&response = client.getResponse();
	std::string		header = response.getHeader();

	if (sock.isCgi(sockNbr, response.getUrl()) && !redir)
		header = headerForCgi(header, client);
	else
		header += "\r\n\r\n";
	
	std::cout << GRAY << std::endl;
	std::cout << header;
	std::cout << END;
	if (send(fde, header.c_str(), header.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}

static size_t chunk_size = 0;

int		treatData(int fde, HTTPResponse &response, size_t size, std::string string, bool isChunked)
{
	if (isChunked)
	{
		std::string			line;
		size_t				size_of_chunk;
		std::ostringstream	os;

		if (response.getMaxSizeC() < size)
			size_of_chunk = response.getMaxSizeC();
		else
			size_of_chunk = size;
		if (size - chunk_size < size_of_chunk)
			size_of_chunk = size - chunk_size;

		os << std::hex << size_of_chunk;
		line += os.str() + "\r\n";
		line += string.substr(chunk_size, size_of_chunk);
		chunk_size += size_of_chunk;
		if (chunk_size >=  size)
			line +=  "0\r\n";
	}
	std::stringstream	fileStream(string, std::ios::in | std::ios::binary);
	char				buf[BUFFER_SIZE];
	int					i;
	char				c;

	if (response.getMethod() == "HEAD" || response.getMethod() == "OPTIONS")
		return (OK);
	while (fileStream.get(c))
	{
		memset(buf, 0, BUFFER_SIZE);
		buf[0] = c;
		i = 1;
		while (fileStream.get(c) && i + 1 < BUFFER_SIZE)
		{
			buf[i] = c;
			i++;
		}
		if (i + 1 == BUFFER_SIZE)
		{
			buf[i] = c;
			i++;
		}
		if (send(fde, buf, i, 0) < 0)
		{
			perror("send()");
			fileStream.str(std::string());
			fileStream.clear();
			return (ERR);
		}
	}
	fileStream.str(std::string());
	fileStream.clear();
	if (isChunked && chunk_size < size)
		treatData(fde, response, size, string, isChunked);
	return (OK);
}

static int	sendData(int fde, Client &client, bool isCgi)
{
	HTTPResponse		&response = client.getResponse();
	std::string			fileName(response.getFileName());
	size_t				size;
	std::stringstream	ss;
	std::string			string;
	bool				isChunked;

	if (isCgi)
	{
		std::string	C = client.getCgiCoprs();
		isChunked = !client.getIsContentLen();
		std::cout <<"chunk = " << isChunked << std::endl;
		size = C.length();
		string = C;
	}
	else
	{
		std::ifstream	fileS(fileName.c_str(), std::ios::in | std::ios::binary);
		ss << response.getContentLen();
		ss >> size;
		char *buff = new char [size + 1];
		fileS.read(buff, size);
		buff[size] = '\0';
		std::string B(buff);
		string = B;
		free(buff);
		fileS.close();
		isChunked = response.isChunked();
	}
	treatData(fde, response, size, string, isChunked);
	return (OK);
}

int		sendResponse(Client &client, Socket &sock, int sockNbr)
{
	HTTPHeader		&header = client.getHeader();
	HTTPResponse	&response = client.getResponse();
	int				fde = client.getFd();
	
	if (response.statusIsOk() && !sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
		response.setStatus("405", " Method Not Allowed", header);
	
	if (getRightFile(sock, sockNbr, client))
	{
		if (response.getRedir() == 1)
		{
			response.rendering(client.getHeader());
			response.setRedir(0);
			if (sendHeader(fde, client, sock, true, sockNbr))
				return (ERR);
			return (OK);
		}
		else if (response.getNeedAutoindex())
			return (sendAutoindexPage(fde, response, response.getUrl(), sock.getRoot(sockNbr, response.getUrl())));
		return (sendDefaultPage(fde, response));
	}
	
	if (sock.isCgi(sockNbr, response.getUrl()))
	{
        header.setContentTypeResponse("text/html");
        response.rendering(header);
		setEnvForCgi(sock, sockNbr, client);
		if (getCGIfile(sock.getCgiPass(sockNbr, response.getUrl()), client) < 0)
			return (ERR);
	}

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

	isDownloading(header, response);

	if (sendHeader(fde, client, sock, false, sockNbr))
		return (ERR);
	if (sendData(fde, client, sock.isCgi(sockNbr, response.getUrl())))
		return (ERR);

	chunk_size = 0;
	// remove("html/tmp.html");
	return (OK);
}
