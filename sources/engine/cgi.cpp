/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/20 17:40:25 by masboula         ###   ########.fr       */
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
	std::map<std::string, std::string>::iterator it = (sock.getEnv()).begin();
	std::string complete;
	size_t	i = 0;
	
	(*env) = (char **)malloc(sizeof(char *) * sock.getEnvSize() + 1);
	if (!(*env))
		return ERR;
	while (i < sock.getEnvSize())
	{
		(*env)[i] = (char *)malloc(sizeof(char) * ((it->first).length() + (it->second).length() + 1));
		if (!(*env)[i])
			return ERR;
		complete = it->first + it->second;
		std::cout << "env = " << complete << std::endl;
		strcpy((*env)[i], complete.c_str());
		i++;
	}
	(*arg) = (char **)malloc(sizeof(char *) * 2);
	if (!(*arg))
		return ERR;
	i = strlen("./bin-cgi/script.sh");
	(*arg)[0] = (char *)malloc(sizeof(char) * (i + 1));
	if (!(*arg)[0])
		return ERR;
	strcpy((*arg)[0], "./bin-cgi/script.sh");
	(*arg)[1] = NULL;
	return OK;
}

int		GetCGIfile(Socket &sock, int sockNbr)
{
	char		**env, **arg;
	pid_t		pid;
	int			status;
	std::string	body;
	
	body = sock.getBody();
	body += "\n\0";
	if (mallocEnv(&env, sock, &arg) < 0)
		return ERR;
	
	int tt = open("file", O_CREAT | O_RDWR);
	write(tt, body.c_str(), body.size());
	
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		dup2(sockNbr, STDOUT_FILENO); // our connected socket become stdout
		execve(arg[0], arg, env);
	}
	else
	{
		close(sockNbr);
		waitpid(pid, &status, 0);
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