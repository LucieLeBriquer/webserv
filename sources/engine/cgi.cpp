/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/22 15:35:58 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

/*******
 * RAPPEL *
 * pipefd[2] : [0] = "read" end of pipe, [1] = "write" end of pipe
 * dup2() give the 2nd arg role to the 1st arg
 * STDIN == 0 & STDOUT == 1
 ********/

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
	size_t i = 0, j = 0, nbr = 0;
	std::string	tmp = sock.getEnv();

	(*env) = (char **)malloc(sizeof(char *) * sock.getEnvSize() + 1);
	if (!(*env))
		return ERR;
	while (tmp[i])
	{
		j = i;
		while (tmp[i] && tmp[i] != '!')
			i++;
		(*env)[nbr] = (char *)malloc(sizeof(char) * ((i - j) + 1));
		if (!(*env)[nbr])
			return ERR;
		strncpy((*env)[nbr], &tmp[j], i - j);
		(*env)[nbr][i - j] = '\0';
		nbr++;
		i++;
	}
	return OK;
}

int 	mallocArg(char ***arg, std::string cgi, std::string file)
{
	(*arg) = (char **)malloc(sizeof(char *) * 3);
	if (!(*arg))
		return ERR;
		
	/** Malloc executable **/
	(*arg)[0] = (char *)malloc(sizeof(char) * (strlen(cgi.c_str()) + 1));
	if (!(*arg)[0])
		return ERR;
	strcpy((*arg)[0], cgi.c_str());
	
	/** Malloc file asked **/
	(*arg)[1] = (char *)malloc(sizeof(char) * (strlen(file.c_str()) + 1));
	if (!(*arg)[1])
		return ERR;
	strcpy((*arg)[1], file.c_str());
	
	/** last arg NULL **/
	(*arg)[2] = NULL;
	return OK;
}

int 	GetCGIfile(Socket &sock, int sockNbr, std::string cgi, std::string file)
{
	char **env, **arg;
	pid_t pid;
	int status;
	std::string body;

	body = sock.getBody();
	body += "\n\0";
	if (mallocEnv(&env, sock) < 0)
		return ERR;
	if (mallocArg(&arg, cgi, file) < 0)
		return ERR;

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
	
	// printing
	std::cout << std::endl;
	std::cout << "================ inside cgi function ===============" << std::endl;
	std::cout << std::endl;
	std::cout << " - arg[0] = " << arg[0] << std::endl;
	std::cout << " - arg[1] = " << arg[1] << std::endl;
	std::cout << " - connected socket = " << sockNbr << std::endl;
	std::cout << " - body = " << sock.getBody() << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << std::endl;
	
	ft_free_env_arg(&env, &arg, &sock);
	return OK;
}