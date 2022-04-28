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

std::string	deletingUseless(std::string header)
{
	int			find, i = 0;
	
	find = header.find("Content-Type:");
	if (find > 0)
	{
		i = find;
		while (header[i] && header[i] != '\n')
			i++;
		header.erase(find, (i + 1) - find);
	}
	find = header.find("Host:");
	if (find > 0)
	{
		i = find;
		while (header[i] && header[i] != '\n')
			i++;
		header.erase(find, (i + 1) - find);
	}
	// find = header.find("Content-Length:");
	// if (find > 0)
	// {
	// 	i = find;
	// 	while (header[i] && header[i] != '\n')
	// 		i++;
	// 	header.erase(find, (i + 1) - find);
	// }
	header += "\r\n";
	return header;
}

std::string	headerForCgi(std::string header, Socket &sock, int socknbr)
{
	std::string	cgiHeader;
	std::string	cgiCorps = sock.getCgiCoprs();
	std::string	tmp;
	int			i = 0;

	cgiHeader = deletingUseless(header);

	cgiHeader += "Server: ";
	cgiHeader += sock.getServerName(socknbr);
	cgiHeader += "\r\n";
	while (cgiCorps[i + 3] && (cgiCorps[i] != '\r' && cgiCorps[i + 1] != '\n' && cgiCorps[i + 2] != '\r' && cgiCorps[i + 3] != '\n'))
		i++;
	tmp = &cgiCorps[i + 6];
	cgiCorps.erase(i + 4, strlen(cgiCorps.c_str()) - (i + 4));
	cgiHeader += cgiCorps;
	cgiHeader += "\r\n";
	sock.setCgiCoprs(tmp);

	return cgiHeader;
}

/*******
 * RAPPEL *
 * pipefd[2] : [0] = "read" end of pipe, [1] = "write" end of pipe
 * dup2() give the 2nd arg role to the 1st arg
 * STDIN == 0 & STDOUT == 1
 ********/

void	setCgiString(FILE *temp, int fdtemp, Socket &sock)
{
	char buff[4096];
	std::string string;

	std::rewind(temp);
	while(!feof(temp))
 	{
       if (fgets(buff, 1024, temp) == NULL)
             break;
       string += buff;
 	}
	close(fdtemp);
	std::fclose(temp);

	sock.setCgiCoprs(string);
}

int 	GetCGIfile(Socket &sock, std::string cgi)
{
	char		**env;
	char        *arg[2] = {(char *)cgi.c_str(), NULL};
	pid_t		pid;
	int			status, fd[2], m = 0;
	FILE		*temp = std::tmpfile();
	int			fdtemp = fileno(temp);
	
	if (mallocEnv(&env, sock) < 0)
		return ERR;

	/////////////////printing////////////////
	std::cout << "======================= ENV ========================" << std::endl;
	std::cout << std::endl;
	mapStr	tmp = sock.getEnv();
	mapStr::iterator it = tmp.begin();
	for (size_t i = 0; env[i]; i++)
	{
		if (it == tmp.end())
			break ;
		std::cout << env[i] << std::endl;
		// std::cout << "s = " << sock.getEnvSize() << "  " << it->first << "=" << it->second << std::endl;
		it++;
	}
	std::cout << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << std::endl;
	/////////////////////////////////////////

	if (sock.getEnvValue("REQUEST_METHOD") == "POST")
	{
		m = POST;
		pipe(fd);
	}
	
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		if (m == POST)
		{
			close(fd[1]);
			if (dup2(fd[0], STDIN_FILENO) < 0)
			{
				perror("dup2()");
				exit(EXIT_FAILURE);
			}
			close(fd[0]);
		}
		if (dup2(fdtemp, STDOUT_FILENO) < 0)
		{
			perror("dup2()");
			exit(EXIT_FAILURE);
		}
		execve((char *)cgi.c_str(), arg, env);
		perror("execve()");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (m == POST)
		{
			close(fd[0]);
			write(fd[1], sock.getBody().c_str(), strlen(sock.getBody().c_str()));
			close(fd[1]);
		}
		waitpid(pid, &status, 0);
	}

	setCgiString(temp, fdtemp, sock);
	ft_free_env_arg(&env, &sock);
	sock.unsetEnv();
	sock.setIsQueryString(false);
	return OK;
}