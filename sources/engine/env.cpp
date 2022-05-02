/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:44:49 by user42            #+#    #+#             */
/*   Updated: 2022/04/27 15:56:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void    setEnvForCgi(Socket &sock, HTTPResponse &response, int sockNbr, HTTPHeader &header)
{
	std::stringstream	out;
	std::string			string;
    
	sock.setEnvValue("SERVER_NAME", sock.getServerName(sockNbr));
    sock.setEnvValue("GATEWAY_INTERFACE", "CGI/1.1");
	sock.setEnvValue("PATH_INFO", sock.getRealUrl(sockNbr, response.getUrl()));
	sock.setEnvValue("REQUEST_METHOD", response.getMethod());
	sock.setEnvValue("SCRIPT_FILENAME", sock.getRealUrl(sockNbr, response.getUrl()));
	sock.setEnvValue("SERVER_PROTOCOL", "HTTP/1.1");
	sock.setEnvValue("REDIRECT_STATUS", "200");
	sock.setEnvValue("CONTENT_TYPE", header.getContentType());
	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
	{
		string = sock.getBody();
		if (string[strlen(string.c_str()) - 1] == '\n' && string[strlen(string.c_str()) - 2] == '\r')
			string.erase(strlen(string.c_str()) - 2, 2);
		sock.setBody(string);
		out << strlen(sock.getBody().c_str());
		sock.setEnvValue("CONTENT_LENGTH", out.str());
	}
	else
	{
		if (sock.isQueryString() == false)
			sock.setEnvValue("QUERY_STRING", "");
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