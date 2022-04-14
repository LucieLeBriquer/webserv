/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/12 16:06:46 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

/*******
 * RAPPEL *
 * pipefd[2] : [0] = "read" end of pipe, [1] = "write" end of pipe
 * dup2() give the 2nd arg role to the 1st arg
 * STDIN == 0 & STDOUT == 1
	********/

int		mallocEnv(char ***env, Socket &sock, char ***arg)
{
	size_t	i = 0;
	
	(*env) = (char **)malloc(sizeof(char *) * sock.getEnvSize() + 1);
	if (!(*env))
		return ERR;
	while (i < sock.getEnvSize())
	{
		(*env)[i] = (char *)malloc(sizeof(char) * ((sock.getEnv(i)).length() + 1));
		if (!(*env)[i])
			return ERR;
		strcpy((*env)[i], (sock.getEnv(i)).c_str());
		i++;
	}
	(*arg) = (char **)malloc(sizeof(char *) * 2);
	if (!(*arg))
		return ERR;
	i = strlen(&sock.getEnv(0)[10]);
	(*arg)[0] = (char *)malloc(sizeof(char) * (i + 1));
	if (!(*arg)[0])
		return ERR;
	strcpy((*arg)[0], &sock.getEnv(0)[10]);
	(*arg)[1] = NULL;
	return OK;
}

int		GetCGIfile(Socket &sock, int sockNbr)
{
	char		**env, **arg;
	pid_t		pid;
	int			fd[2], socket;
	std::string	body;
	
	/********************************************/
	sock.setEnv("PATH_INFO=/home/user42/Documents/42/webserv/bin-cgi/env.pl");
	sock.setEnv("CONTENT_TYPE=application/x-www-form-urlencoded"); // need get content type
	sock.setEnv("CONTENT_LENGTH=13"); //need get content length
	sock.setEnv("REQUEST_METHODE=POST");
	/********************************************/
	pipe(fd);
	socket = sockNbr;
	body = sock.getBody();
	pid = fork();
	if (mallocEnv(&env, sock, &arg) < 0)
		return ERR;
	
	if (pid < 0)
		exit(EXIT_FAILURE);
	else if (pid == 0) //inside child process
	{
		close(fd[1]); // closing "write" side
		dup2(fd[0], STDIN_FILENO); // "read" side become stdin
		dup2(socket, STDOUT_FILENO); // our connected socket become stdout
		execve(arg[0], arg, env);
	}
	else //inside parent process, we have to send the body through our pipefd[1]
	{
		close(fd[0]); // closing "read" side
		write(fd[1], body.c_str(), body.size()); // write() body on the "write" side
	}
	
	//printing
	std::cout << "inside cgi function : " << std::endl;
	size_t i = 0;
	while (i < sock.getEnvSize())
	{
		std::cout << "env[" << i << "] = " << env[i] << std::endl;
		i++;
	}
	std::cout << " - arg[0] = " << arg[0] << std::endl;
	std::cout << " - connected socket = " << sockNbr << std::endl;
	std::cout << " - body = " << sock.getBody() << std::endl;
	return OK;
}