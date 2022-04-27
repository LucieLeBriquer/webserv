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

void    setEnvForCgi(Socket &sock, HTTPResponse &response, int sockNbr)
{
	std::stringstream out;
    
	sock.setEnvValue("SERVER_NAME", "webserv");
    sock.setEnvValue("GATEWAY_INTERFACE", "CGI/1.1");
	sock.setEnvValue("PATH_INFO", sock.getRealUrl(sockNbr, response.getUrl()));
	sock.setEnvValue("REQUEST_METHOD", response.getMethod());
	sock.setEnvValue("SCRIPT_FILENAME", sock.getRealUrl(sockNbr, response.getUrl()));
	sock.setEnvValue("SERVER_PROTOCOL", "HTTP/1.1");
	sock.setEnvValue("REDIRECT_STATUS", "200");
	sock.setEnvValue("CONTENT_TYPE", "application/x-www-form-urlencoded");
	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
	{
		out << strlen(sock.getBody().c_str());
		sock.setEnvValue("CONTENT_LENGTH", out.str());
	}
	else
	{
		if (sock.isQueryString() == false)
			sock.setEnvValue("QUERY_STRING", "");
	}
}

void	ft_free_env_arg(char ***env, char ***arg, Socket *sock)
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
	i = 0;
	while (i < 3)
	{
		free((*arg)[i]);
		(*arg)[i] = NULL;
		i++;
	}
	free((*arg));
	(*arg) = NULL;
}

int 	mallocEnv(char ***env, Socket &sock)
{
	size_t				nbr = 0;
	mapStr				tmp = sock.getEnv();
	std::string			val;

	(*env) = (char **)malloc(sizeof(char *) * sock.getEnvSize() + 1);
	if (!(*env))
		return ERR;
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

int 	mallocArg(char ***arg, std::string cgi, std::string file)
{
	(*arg) = (char **)malloc(sizeof(char *) * 1);
	if (!(*arg))
		return ERR;
	
	(void)cgi;
	(void)file;
	// /** Malloc executable **/
	// (*arg)[0] = (char *)malloc(sizeof(char) * (strlen(cgi.c_str()) + 1));
	// if (!(*arg)[0])
	// 	return ERR;
	// strcpy((*arg)[0], cgi.c_str());
	
	// /** Malloc file asked **/
	// (*arg)[1] = (char *)malloc(sizeof(char) * (strlen(file.c_str()) + 1));
	// if (!(*arg)[1])
	// 	return ERR;
	// strcpy((*arg)[1], file.c_str());
	
	/** last arg NULL **/
	// (*arg)[0] = (char *)malloc(sizeof(char) * 1);
	// if (!(*arg)[0])
	// 	return ERR;
	(*arg)[0] = NULL;
	return OK;
}