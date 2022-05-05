/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/05 15:30:01 by lle-briq         ###   ########.fr       */
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
	header.setContentTypeResponse(mimeContentType(header.getAccept(), filename));

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
		std::ifstream	fileStream(response.getFileName().c_str(), std::ios::in | std::ios::binary);
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
	return (OK);
}

int		sendResponse(int fde, HTTPResponse &response, HTTPHeader &header, Socket &sock, int sockNbr)
{
	if (response.getStatusNb() == 0
		&& !sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
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
			return (ERR);
	}

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde;
	std::cout << " from " << ORANGE << sock.getRealUrl(sockNbr, response.getUrl()) << END << std::endl;

	if (sendHeader(fde, response, sock, false, sockNbr))
		return (ERR);
	if (sendData(fde, response, sock.isCgi(sockNbr, response.getUrl()), sock))
		return (ERR);

	if ((response.getStatusNb() != 200 && response.getStatusNb() != 0) || sock.isCgi(sockNbr, response.getUrl()))
		close(fde);

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
	if (header.header(string) == -1)
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

int		requestReponse(int epollfd, int fde, Socket *sock)
{
	HTTPResponse	response;
	HTTPHeader		header;
	Status			status;
	std::string		string;
	char			buf[BUFFER_SIZE];
	int				byteCount = 0;
	int				recvLen = 0;
	int				line = 0;
	int				isBreak = 0;
	int				sockNbr = sock->getConnection(fde);

	while (1)
	{
		memset(buf, 0, BUFFER_SIZE);
		byteCount = recv(fde, buf, BUFFER_SIZE, 0);

		if (byteCount == 0 || (byteCount == 1 && (int)buf[0] == 4))
		{
			isBreak = 1;
			break ;
		}
		else if (byteCount < 0)
		{
			if (line == 0 && !onlySpaces(string))
			{
				line++;
				if (header.method(string, &status, &response) == -1)
				{
					isBreak = 2;
					break ;
				}
			}
			if (!onlySpaces(string) && endRequest(string, *sock))
				break ;
		}
		else
		{
			if (!onlySpaces(buf) || line > 0)
			{
				recvLen += byteCount;
				std::cout << GREEN << "[Received] " << END << recvLen << " bytes from " << fde << std::endl;
				std::cout << "====================================================" << std::endl;
				std::cout << buf ;
				std::cout << "\n====================================================" << std::endl << std::endl;
				string += buf;
			}
		}
	}
	if (isBreak != 1)
	{
		if (checkHeader(header, string) == -1)
			status.statusCode(status.status(4, 0), header.getFirstLine());
		header.setContentTypeResponse(mimeContentType(header.getAccept(), header.getUrl()));
		response.setServerName(sock->getServerName(sockNbr));
		if (sendResponse(fde, response, header, *sock, sockNbr))
			return (ERR);
	}
	if (isBreak > 0)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, fde, NULL);
		close(fde);
	}
	return (OK);
}