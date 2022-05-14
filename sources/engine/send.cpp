/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/14 14:26:08 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static int	sendHeader(int fde, Client &client, HTTPResponse &response, Socket &sock, bool redir, int sockNbr)
{
	std::string	header = response.getHeader();

	if (sock.isCgi(sockNbr, response.getUrl()) && !redir)
		header = headerForCgi(header, client, sock, sockNbr);
	else if (redir)
		header += "\r\n\r\n\r\n";
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

static int	sendData(int fde, Client &client, HTTPResponse &response, bool isCgi, Socket &sock)
{
	std::string		fileName(response.getFileName());
	std::string		tmpname("html/tmp.html");
	std::stringstream ss;
	size_t			size;
	ss << response.getContentLen();
	ss >> size;
	if (response.isChunked())
	{
		// std::cout << "ok 1.0" << std::endl;
		std::ifstream	fileS(fileName.c_str(), std::ios::in | std::ios::binary);
		std::ofstream	tmpfile(tmpname.c_str());
		std::string		line;
		size_t			i;
		size_t			size_of_chunk;

		// std::cout << "max size = " << response.getMaxSizeC() << std::endl;
		// std::cout << "size = " << size << std::endl;
		// std::cout << "chunk size = " << chunk_size << std::endl;
		if (response.getMaxSizeC() < size)
			size_of_chunk = response.getMaxSizeC();
		else
			size_of_chunk = size;

		char *buff = new char [size_of_chunk + 1];
		for (i = 0; i < chunk_size; i += size_of_chunk )
			fileS.read(buff, size_of_chunk);

		if (size - chunk_size < size_of_chunk)
			size_of_chunk = size - chunk_size;
		tmpfile << std::hex << size_of_chunk << "\r\n";
		fileS.read(buff, size_of_chunk);
		buff[size_of_chunk] = '\0';

		tmpfile << buff << "\r\n";
		free(buff);

		chunk_size += size_of_chunk;
		if (chunk_size >=  size)
			tmpfile << "0" << "\r\n";
		fileName = tmpname;
		response.setUrl(tmpname);
		tmpfile.close();
		fileS.close();		
		// seg fault when max = 1
	}
	if (isCgi)
	{
		std::stringstream	fileStream(client.getCgiCoprs(), std::ios::in | std::ios::binary);
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
	}
	else
	{
		std::ifstream	fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
		char			buf[BUFFER_SIZE];
		int				i;
		char			c;
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
				fileStream.close();
				return (ERR);
			}
		}
		fileStream.close();
	}
	if (response.isChunked() && chunk_size < size)
		sendData(fde, client, response, isCgi, sock);
	return (OK);
}

int		sendResponse(Client &client, Socket &sock, int sockNbr)
{
	HTTPHeader		&header = client.getHeader();
	HTTPResponse	&response = client.getResponse();
	int				fde = client.getFd();

	if (response.getStatusNb() == 0
		&& !sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
		response.setStatus("405", " Method Not Allowed", header);

	if (getRightFile(sock, sockNbr, client))
	{
		if (response.getRedir() == 1)
		{
			response.rendering(client.getHeader());
			response.setRedir(0);
			if (sendHeader(fde, client, response, sock, true, sockNbr))
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
			return ERR;
	}

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

	isDownloading(header, response);

	if (sendHeader(fde, client, response, sock, false, sockNbr))
		return (ERR);
	if (sendData(fde, client, response, sock.isCgi(sockNbr, response.getUrl()), sock))
		return (ERR);

	if ((response.getStatusNb() != 200 && response.getStatusNb() != 0) || sock.isCgi(sockNbr, response.getUrl()))
		close(fde);

	remove("html/tmp.html");

	return (OK);
}
