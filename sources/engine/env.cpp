/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:44:49 by user42            #+#    #+#             */
/*   Updated: 2022/05/09 14:13:16 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void    setEnvForCgi(Socket &sock, HTTPResponse &response, int sockNbr, HTTPHeader &header)
{
	std::stringstream	out;
	std::string			string;
	std::string			file = sock.getRealUrl(sockNbr, response.getUrl());
    
	if (strcmp(file.c_str(), "html/upload.php") == 0)
	{
		std::rewind(sock.getBody());
		char	buf[BUFFER_SIZE + 1];
		int		ret;

		while ((ret = read(sock.getFdBody(), buf, BUFFER_SIZE)) > 0)
		{
			buf[ret] = '\0';
			// std::cout << "buf = [" << buf << "]" << std::endl;
		}
	}
	sock.setEnvValue("SERVER_NAME", sock.getServerName(sockNbr));
    sock.setEnvValue("GATEWAY_INTERFACE", "CGI/1.1");
	sock.setEnvValue("PATH_INFO", file);
	sock.setEnvValue("REQUEST_METHOD", response.getMethod());
	sock.setEnvValue("SCRIPT_FILENAME", file);
	sock.setEnvValue("SERVER_PROTOCOL", "HTTP/1.1");
	sock.setEnvValue("REDIRECT_STATUS", "200");
	if (header.getContentType() == "")
		sock.setEnvValue("CONTENT_TYPE", "application/x-www-form-urlencoded");
	else
		sock.setEnvValue("CONTENT_TYPE", header.getContentType());
	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
	{
		std::rewind(sock.getBody());
		out << getFdSize(sock.getFdBody());
		sock.setEnvValue("CONTENT_LENGTH", out.str());
	}
	else
	{
		if (sock.isQueryString() == false)
			sock.setEnvValue("QUERY_STRING", "");
	}
	std::rewind(sock.getBody());
	char	buf[BUFFER_SIZE + 1];
	int		ret;
	
	while ((ret = read(sock.getFdBody(), buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		// std::cout << "buf = [" << buf << "]" << std::endl;
	}
}

void	ft_free_env_arg(char ***env, Socket *sock)
{
	size_t i = 0;

	while (i < sock->getEnvSize())
	{
		free((*env)[i]);
		(*env)[i] = NULL;
		i++;
	}
	free((*env));
	(*env) = NULL;
}

int 	mallocEnv(char ***env, Socket &sock)
{
	size_t				nbr = 0;
	mapStr				tmp = sock.getEnv();
	std::string			val;

	(*env) = (char **)malloc(sizeof(char *) * sock.getEnvSize() + 1);
	if (!(*env))
		return ERR;
	(*env)[sock.getEnvSize()] = NULL;
	for (mapStr::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		val = it->first;
		val += "=";
		val += it->second;
		(*env)[nbr] = (char *)malloc(sizeof(char) * (strlen(val.c_str()) + 1));
		if (!(*env)[nbr])
			return ERR;
		strcpy((*env)[nbr], val.c_str());
		(*env)[nbr][strlen(val.c_str())] = '\0';
		nbr++;
	}
	return OK;
}