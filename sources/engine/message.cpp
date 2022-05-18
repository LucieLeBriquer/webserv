/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:15:59 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/18 14:29:37 by masboula         ###   ########.fr       */
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
		}
		if (endRequest(client) && end == 0)
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

	std::cout << GREEN << "[Received] " << END << byteCount << " bytes from " << fde << std::endl;
	
	if (byteCount == 0)
		end = CLOSE_CONNECTION;
	else if (byteCount < 0)
		checkFirstAndEnd(end, client);
	else if (!onlySpaces(buf) || !client.isFirstLine())
	{
		client.addRecv(buf, byteCount);
		checkFirstAndEnd(end, client);
	}
	
	if (end == BAD_REQUEST)
		status.statusCode(status.status(4, 0), header.getFirstLine());
	if (end == BAD_REQUEST || end == END_REQUEST)
	{
		if (checkHeader(header, client.getRequest()))
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
		std::cerr << std::endl << RED << "[Closing]" << END << " connection with " << fde << std::endl;
		client.clear();
		sock.removeClient(fde);
		epoll_ctl(sock.getEpollFd(), EPOLL_CTL_DEL, fde, NULL);
		close(fde);
	}
	return (OK);
}
