/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/12 17:11:44 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static int	getRightFile(Socket &sock, int sockNbr, Client &client)
{
	std::string 		filename;
	size_t				size;
	HTTPResponse		&response = client.getResponse();
	HTTPHeader			&header = client.getHeader();

	filename = response.redirect(sock, sockNbr, header, client);
	if (filename == "")
		return (ERR);

	response.setFileName(filename);
	header.setContentTypeResponse(mimeContentType(header.getAccept(), filename));

	//---
	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();
	//---> replace by fdSize()
	response.setContentLen(size);
	response.rendering(header);
	return (OK);
}

static int	sendHeader(int fde, HTTPResponse &response, Socket &sock, bool redir, int sockNbr)
{
	std::string	header = response.getHeader();

	if (sock.isCgi(sockNbr, response.getUrl()) && !redir)
		header = headerForCgi(header, sock, sockNbr);
	else if (redir)
		header += "\r\n\r\n\r\n";
	else
		header += "\r\n\r\n";
	
	std::cout << "====================================================" << std::endl;
	std::cout << header;
	std::cout << "====================================================" << std::endl;
	if (send(fde, header.c_str(), header.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}

static size_t chunk_size = 0;

static int	sendData(int fde, HTTPResponse &response, bool isCgi, Socket &sock)
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
		std::stringstream	fileStream(sock.getCgiCoprs(), std::ios::in | std::ios::binary);
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
		sendData(fde, response, isCgi, sock);
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
			if (sendHeader(fde, response, sock, true, sockNbr))
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
		if (getCGIfile(sock, sock.getCgiPass(sockNbr, response.getUrl()), client) < 0)
			return ERR;
	}

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

	isDownloading(header, response);

	if (sendHeader(fde, response, sock, false, sockNbr))
		return (ERR);
	if (sendData(fde, response, sock.isCgi(sockNbr, response.getUrl()), sock))
		return (ERR);

	if ((response.getStatusNb() != 200 && response.getStatusNb() != 0) || sock.isCgi(sockNbr, response.getUrl()))
		close(fde);

	remove("html/tmp.html");

	return (OK);
}

int		checkHeader(HTTPHeader &header, std::string string)
{
	string.erase(0, (getHead(string)).length() + 2);
	while (1)
	{
		if (string == "")
			break ;
		if (header.fillheader(&string) == -1)
			break ; // a changer en fonction du retour d'err
	}
	if (header.header() == -1)
		return ERR;
	return 1;
}

static bool	onlySpaces(const std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] != '\n' && str[i] != '\r')
			return (false);
	}
	return (true);
}

static bool	onlySpaces(const char *str)
{
	for (size_t i = 0; str[i]; i++)
	{
		if (str[i] != '\n' && str[i] != '\r')
			return (false);
	}
	return (true);
}

static void	checkFirstAndEnd(int &end, Client &client, Socket &sock)
{
	HTTPResponse	&response = client.getResponse();
	HTTPHeader		&header = client.getHeader();
	Status			&status = client.getStatus();

	if (!onlySpaces(client.getRequest()))
	{
		if (client.isFirstLine())
		{
			client.changeFirstLine();
			if (header.method(client.getRequest(), status, response) == -1)
				end = BAD_REQUEST;
		}
		if (endRequest(client.getTmp(), sock, client.getRequest()) && end == 0)
			end = END_REQUEST;
	}
}

int		requestReponse(int fde, Socket &sock)
{
	char			buf[BUFFER_SIZE + 1];
	int				byteCount = 0;
	int				end = 0;
	int				sockNbr = sock.getConnection(fde);

	if (!sock.isConnectedClient(fde))
		sock.addClient(fde);
	Client			&client = sock.getClient(fde);
	HTTPResponse	&response = client.getResponse();
	HTTPHeader		&header = client.getHeader();
	Status			&status = client.getStatus();
	
	memset(buf, 0, BUFFER_SIZE);
	byteCount = recv(fde, buf, BUFFER_SIZE, 0);
	if (byteCount > 0)
		buf[byteCount] = 0;

	std::cout << GREEN << "[Received] " << END << byteCount << " bytes from " << fde << std::endl << std::endl;

	if (byteCount == 0)
		end = CLOSE_CONNECTION;
	else if (byteCount < 0)
		checkFirstAndEnd(end, client, sock);
	else if (!onlySpaces(buf) || !client.isFirstLine())
	{
		client.addRecv(buf, byteCount);
		checkFirstAndEnd(end, client, sock);
	}

	if (end == BAD_REQUEST)
		status.statusCode(status.status(4, 0), header.getFirstLine());
	if (end == BAD_REQUEST || end == END_REQUEST)
	{
			if (checkHeader(header, client.getRequest()) == -1)
				status.statusCode(status.status(4, 0), header.getFirstLine());
			header.setContentTypeResponse(mimeContentType(header.getAccept(), header.getUrl()));
			response.setServerName(sock.getServerName(sockNbr));
			response.setMaxSizeC(sock.getMaxClientBodySize(sockNbr, response.getUrl()));
			if (sendResponse(client, sock, sockNbr))
				return (ERR);
			client.clear();
	}
	if (end == BAD_REQUEST || end == CLOSE_CONNECTION)
	{
		std::cerr << "closing connection with " << fde << std::endl;
		client.clear();
		sock.removeClient(fde);
		epoll_ctl(sock.getEpollFd(), EPOLL_CTL_DEL, fde, NULL);
		close(fde);
	}
	return (OK);
}
