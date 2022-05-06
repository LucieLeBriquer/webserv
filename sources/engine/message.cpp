/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/06 16:33:27 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

static int	getRightFile(HTTPResponse &response, Socket &sock, int sockNbr, HTTPHeader &header)
{
	std::string 		filename;
	size_t				size;

	size = 0;
	filename = response.redirect(sock, sockNbr, header);
	if (filename == "")
		return (ERR);
	response.setFileName(filename);

	std::ifstream		fileStream(filename.c_str(), std::ios::in | std::ios::binary);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
	fileStream.close();
	response.setContentLen(size);
	response.rendering(header);
	return (OK);
}

static int	sendHeader(int fde, HTTPResponse &response, Socket &sock, bool redir, int sockNbr)
{
	std::string	header = response.getHeader();

	if (sock.isCgi(sockNbr, response.getUrl()) && !redir)
		header = headerForCgi(header, sock);
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

static int	sendData(int fde, HTTPResponse &response, bool isCgi, Socket &sock)
{
	std::string		fileName(response.getFileName());
	std::string		tmpname("html/tmp.html");
	std::stringstream ss;
	int				size;
	ss << response.getContentLen();
	ss >> size;

	if (response.isChunked())
	{
		std::ifstream	fileS(fileName.c_str(), std::ios::in | std::ios::binary);
		std::ofstream	tmpfile(tmpname.c_str());
		std::string		line;
		int				i;
		size_t			size_of_chunk;

		// std::cout << "max size = " << response.getMaxSizeC() << std::endl;
		// std::cout << "size = " << size << std::endl;
		if (response.getMaxSizeC() < size)
			size_of_chunk = response.getMaxSizeC();
		else
			size_of_chunk = size;

		char *buff = new char [size_of_chunk + 1];
		for (i = 0; i < chunk_size; i += size_of_chunk )
			fileS.read(buff, size_of_chunk);

		tmpfile << std::hex << size_of_chunk << "\r\n";

		fileS.read(buff, size_of_chunk);
		buff[size_of_chunk] = '\0';
		
		tmpfile << buff << "\r\n";
		
		free(buff);
		chunk_size += size_of_chunk;

		if (chunk_size >= size)
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

int		sendResponse(int fde, HTTPResponse &response, HTTPHeader &header, Socket &sock, int sockNbr)
{
	if (!sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
		response.setStatus("405", " Method Not Allowed", header);

	if (getRightFile(response, sock, sockNbr, header))
	{
		if (response.getRedir() == 1)
		{
			response.rendering(header);
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

		setEnvForCgi(sock, response, sockNbr, header);
		if (getCgiFile(sock, sock.getCgiPass(sockNbr, response.getUrl())) < 0)
			return ERR;
	}

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

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

int		requestReponse(int epollfd, int fde, Socket *sock)
{
	char			buf[BUFFER_SIZE] = {0};
	int				byteCount, recv_len = 0;
	std::string		string;
	HTTPResponse	response;
	HTTPHeader		header;
	Status			status;
	int				line(0), isBreak = 0;
	int				sockNbr = sock->getConnection(fde);

	while (1)
	{
		memset(buf, 0, BUFFER_SIZE);
		byteCount = recv(fde, buf, BUFFER_SIZE, 0);
		if (byteCount == 0)
		{
			std::cout << "stops ??" << std::endl;
			epoll_ctl(epollfd, EPOLL_CTL_DEL, fde, NULL);
			isBreak = 1;
			break ;
		}
		else if (byteCount < 0)
		{
			if (line == 0)
			{
				if (header.method(string, &status, &response) == -1)
					break ;
			}
			if (endRequest(string, *sock))
				break ;
			line++;
		}
		else
		{
			recv_len += byteCount;
			std::cout << GREEN << "[Received] " << END << recv_len << " bytes from " << fde << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << buf ;
			std::cout << "\n====================================================" << std::endl << std::endl;
		}
		string += buf;
	}
	if (isBreak == 0)
	{
		if (checkHeader(header, string) == -1)
			status.statusCode(status.status(4, 0), header.getFirstLine());
		header.setContentTypeResponse(mimeContentType(header.getAccept(), header.getUrl()));
		response.setMaxSizeC(sock->getMaxClientBodySize(sockNbr, response.getUrl()));
		if (sendResponse(fde, response, header, *sock, sockNbr))
			return (ERR);
	}
	return (OK);
}