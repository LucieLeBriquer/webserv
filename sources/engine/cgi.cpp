/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/26 14:15:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

std::string	headerForCgi(std::string header)
{
	std::string	cgiHeader;
	int			find, i;

	cgiHeader = header;
	// find = cgiHeader.find("Content-Type:");
	// i = find;
	// while (cgiHeader[i] && cgiHeader[i] != '\n')
	// 	i++;
	// cgiHeader.erase(find, (i + 1) - find);
	find = cgiHeader.find("Host:");
	i = find;
	while (cgiHeader[i] && cgiHeader[i] != '\n')
		i++;
	cgiHeader.erase(find, (i + 1) - find);
	find = cgiHeader.find("Content-Length:");
	i = find;
	while (cgiHeader[i] && cgiHeader[i] != '\n')
		i++;
	cgiHeader.erase(find, (i + 1) - find);
	cgiHeader += "\r\n";
	return cgiHeader;
}

/*******
 * RAPPEL *
 * pipefd[2] : [0] = "read" end of pipe, [1] = "write" end of pipe
 * dup2() give the 2nd arg role to the 1st arg
 * STDIN == 0 & STDOUT == 1
 ********/

int 	GetCGIfile(Socket &sock, int fde, std::string cgi, std::string file)
{
	char		**env, **arg;
	pid_t		pid;
	int			status, fd[2], fdf;
	std::string	body;

	fdf = fde;
	body = sock.getBody();
	body += "\n\0";
	if (mallocEnv(&env, sock) < 0)
		return ERR;
	if (mallocArg(&arg, cgi, file) < 0)
		return ERR;

	/////////////////printing////////////////
	std::cout << "=== ENV ===" << std::endl;
	mapStr	tmp = sock.getEnv();
	mapStr::iterator it = tmp.begin();
	for (size_t i = 0; env[i]; i++)
	{
		if (it == tmp.end())
			break ;
		std::cout << "i = " << i << "  " << env[i] << std::endl;
		// std::cout << "s = " << sock.getEnvSize() << "  " << it->first << "=" << it->second << std::endl;
		it++;
	}
	std::cout << "===========" << std::endl;
	/////////////////////////////////////////
	
	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
		pipe(fd);
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		if (sock.getEnvValue("REQUEST_METHOD") == "POST")
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
		}
		dup2(fdf, STDOUT_FILENO); // our connected socket become stdout
		execve(arg[0], arg, env);
	}
	else
	{
		if (sock.getEnvValue("REQUEST_METHOD") == "POST")
		{
			write(fd[1], sock.getBody().c_str(), strlen(sock.getBody().c_str()));
		}
		close(fd[0]);
		close(fd[1]);
		close(fdf);
		waitpid(pid, &status, 0);
	}
	
	/////////////////printing////////////////
	std::cout << std::endl;
	std::cout << "================ inside cgi function ===============" << std::endl;
	std::cout << std::endl;
	std::cout << " - arg[0] = " << arg[0] << std::endl;
	std::cout << " - arg[1] = " << arg[1] << std::endl;
	std::cout << " - connected socket = " << fdf << std::endl;
	std::cout << " - body = " << sock.getBody() << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << std::endl;
	/////////////////////////////////////////
	
	ft_free_env_arg(&env, &arg, &sock);
	sock.unsetEnv();
	sock.setIsQueryString(false);
	return OK;
}