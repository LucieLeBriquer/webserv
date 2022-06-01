/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 17:44:49 by user42            #+#    #+#             */
/*   Updated: 2022/05/14 17:46:01 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void    setEnvForCgi(Socket &sock, int sockNbr, Client &client)
{
	std::string			string;
	HTTPResponse		&response = client.getResponse();
	HTTPHeader			&header = client.getHeader();
	std::string			file = sock.getRealUrl(sockNbr, response.getUrl());
    
	client.setEnvValue("SERVER_NAME", sock.getServerName(sockNbr));
    client.setEnvValue("GATEWAY_INTERFACE", "CGI/1.1");
	client.setEnvValue("PATH_INFO", file);
	client.setEnvValue("REQUEST_METHOD", response.getMethod());
	client.setEnvValue("SCRIPT_FILENAME", file);
	client.setEnvValue("SERVER_PROTOCOL", "HTTP/1.1");
	client.setEnvValue("REDIRECT_STATUS", "200");
	if (header.getContentType() == "")
		client.setEnvValue("CONTENT_TYPE", "application/x-www-form-urlencoded");
	else
		client.setEnvValue("CONTENT_TYPE", header.getContentType());
	if (client.getEnvValue("REQUEST_METHOD") == "POST")
		client.setEnvValue("CONTENT_LENGTH", toString(client.getBodySize()));
	else
	{
		if (client.isQueryString() == false)
			client.setEnvValue("QUERY_STRING", "");
	}
}

void	freeEnv(char ***env, Client &client)
{
	size_t i = 0;

	while (i < client.getEnvSize())
	{
		free((*env)[i]);
		(*env)[i] = NULL;
		i++;
	}
	free((*env));
	(*env) = NULL;
}

int 	mallocEnv(char ***env, Client &client)
{
	size_t				nbr = 0;
	mapStr				tmp = client.getEnv();
	std::string			val;

	(*env) = (char **)malloc(sizeof(char *) * client.getEnvSize() + 1);
	if (!(*env))
		return (ERR);
	(*env)[client.getEnvSize()] = NULL;
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
	return (OK);
}