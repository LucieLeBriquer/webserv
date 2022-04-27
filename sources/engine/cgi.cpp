/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/27 15:57:09 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

std::string	headerForCgi(std::string header)
{
	std::string	cgiHeader;
	int			find, i;

	cgiHeader = header;
	find = cgiHeader.find("Content-Type:");
	i = find;
	while (cgiHeader[i] && cgiHeader[i] != '\n')
		i++;
	cgiHeader.erase(find, (i + 1) - find);
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
	char		**env;
	std::string n = "NULL";
	char		*arg[] = {(char *)n.c_str()};
	pid_t		pid;
	int			status;
	int			fd[2];
	int			method = 0;
	
	(void)file;
	if (mallocEnv(&env, sock) < 0)
		return ERR;
	// if (mallocArg(&arg, cgi, file) < 0)
		// return ERR;

	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
		method = 1;
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
	
	FILE *temp = std::tmpfile();
	int	fdtemp = fileno(temp);

	if (method == 1)
		pipe(fd);
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		if (method == 1)
		{
			// FILE *post = std::tmpfile();
			// std::fputs(sock.getBody().c_str(), post);
			// std::rewind(post);
			// dup2(fileno(post), STDIN_FILENO);
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}
		// dup2(fde, STDOUT_FILENO);
		dup2(fdtemp, STDOUT_FILENO); // our connected socket become stdout
		execve((char *)cgi.c_str(), arg, env);
		perror("execve()");
	}
	else
	{
		if (method == 1)
		{
			close(fd[0]);
			write(fd[1], sock.getBody().c_str(), strlen(sock.getBody().c_str()));
			close(fd[1]);
		}
		// close(fde);
		waitpid(pid, &status, 0);
	}

	std::rewind(temp);
	char buff[4096];
	std::string string;
	while(!feof(temp))
 	{
       if (fgets(buff, 1024, temp) == NULL)
             break;
       string += buff;
 	}
	close(fdtemp);
	std::fclose(temp);
	sock.setCgiCoprs(string);
	
	/////////////////printing////////////////
	std::cout << std::endl;
	std::cout << "================ inside cgi function ===============" << std::endl;
	std::cout << std::endl;
	std::cout << " - arg[0] = " << arg[0] << std::endl;
	// std::cout << " - arg[1] = " << arg[1] << std::endl;
	std::cout << " - connected socket = " << fde << std::endl;
	std::cout << " - body = " << sock.getBody() << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << std::endl;
	/////////////////////////////////////////
	
	// ft_free_env_arg(&env, &arg, &sock);
	sock.unsetEnv();
	sock.setIsQueryString(false);
	return OK;
}