/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/27 14:00:54 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int		getRightFile(Socket &sock, int sockNbr, Client &client)
{
	std::string 		filename;
	HTTPResponse		&response = client.getResponse();
	HTTPHeader			&header = client.getHeader();

	filename = response.redirect(sock, sockNbr, header, client);
	if (filename == "")
		return (ERR);

	response.setFileName(filename);
	header.setContentTypeResponse(mimeContentType(header.getAccept(), filename));
	response.setContentLen(getFileSize(filename));
	response.rendering(header);
	return (OK);
}

int		checkHeader(HTTPHeader &header, const std::string string)
{
	std::string	copy = string;

	copy.erase(0, (getHead(copy)).length() + 2);
	while (1)
	{
		if (copy == "")
			break ;
		if (header.fillheader(copy) == -1)
			break ;
	}
	if (header.header() == -1)
		return (ERR);
	return (OK);
}

static void	checkFirstAndEnd(int &end, Client &client)
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
			client.updateMethod();
		}
		if (endRequest(client) && end == 0)
			end = END_REQUEST;
	}
}

static int	treatEndRequest(Client &client, int &end, Socket &sock, int sockNbr, int fde)
{
	HTTPResponse	&response = client.getResponse();
	HTTPHeader		&header = client.getHeader();
	Status			&status = client.getStatus();

	if (end == BAD_REQUEST)
		response.statusCode(status.status(4, 0), header.getFirstLine());
	if (end == BAD_REQUEST || end == END_REQUEST)
	{
		if (checkHeader(header, client.getRequest()))
			response.statusCode(status.status(4, 0), header.getFirstLine());
		else if (client.getBodySize() > sock.getMaxClientBodySize(sockNbr, response.getUrl()))
			response.statusCode(status.status(4, 13), header.getFirstLine());
		header.setContentTypeResponse(mimeContentType(header.getAccept(), header.getUrl()));
		response.setServerName(sock.getServerName(sockNbr));
		response.setMaxSizeC(sock.getMaxClientBodySize(sockNbr, response.getUrl()));
		if (sendResponse(client, sock, sockNbr))
			return (ERR);
		if (!response.statusIsOk())
			end = CLOSE_CONNECTION;
		client.clear();
	}
	if (end == BAD_REQUEST || end == CLOSE_CONNECTION)
	{
		if (LOG_LEVEL >= LVL_INFO)
			std::cout << RED << "[Closing]" << END << " connection with " << fde << std::endl;
		client.clear();
		sock.removeClient(fde);
		epoll_ctl(sock.getEpollFd(), EPOLL_CTL_DEL, fde, NULL);
		close(fde);
	}
	return (OK);
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

	memset(buf, 0, BUFFER_SIZE);	
	byteCount = recv(fde, buf, BUFFER_SIZE, 0);
	if (byteCount > 0)
		buf[byteCount] = 0;
		
	if (LOG_LEVEL >= LVL_INFO)
		std::cout << GREEN << "[Received] " << END << byteCount << " bytes from " << fde << std::endl;

	if (byteCount == 0)
		end = CLOSE_CONNECTION;
	else if (byteCount < 0)
	{
		checkFirstAndEnd(end, client);
		end = END_REQUEST;
	}
	else if (!onlySpaces(buf) || !client.isFirstLine())
	{
		client.addRecv(buf, byteCount);
		checkFirstAndEnd(end, client);
	}

	return (treatEndRequest(client, end, sock, sockNbr, fde));
}
