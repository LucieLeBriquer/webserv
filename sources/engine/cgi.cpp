/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/06/01 17:42:07 by lle-briq         ###   ########.fr       */
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
	find = header.find("Content-Length:");
	if (find > 0)
	{
		i = find;
		while (header[i] && header[i] != '\n')
			i++;
		header.erase(find, (i + 1) - find);
	}
	return header;
}

std::string	headerForCgi(std::string header, Client &client)
{
	std::string			cgiHeader;
	std::string			cgiBody = client.getCgiBody();
	std::string			tmp;
	std::stringstream	out;
	int			i = 0;
	size_t		find;

	cgiHeader = deletingUseless(header);
	find = cgiBody.find("Content-type:");
	if (find != std::string::npos)
	{
		while (cgiBody[i + 3] && (cgiBody[i] != '\r' && cgiBody[i + 1] != '\n' && cgiBody[i + 2] != '\r' && cgiBody[i + 3] != '\n'))
			i++;
		tmp = &cgiBody[i + 6];
		cgiBody.erase(i + 4, strlen(cgiBody.c_str()) - (i + 4));
	}
	else
		tmp = cgiBody;
	client.setCgiBody(tmp);
	out << client.getCgiBody().length();
	cgiHeader += "Content-Length: ";
	cgiHeader += out.str();
	cgiHeader += "\r\n";
	cgiHeader += cgiBody;
	cgiHeader += "\r\n";
	client.setIsContentLen(cgiHeader);
	return (cgiHeader);
}

static void	setCgiString(FILE *temp, int fdtemp, Client &client)
{
	char buff[4096];
	std::string string;

	rewind(temp);
	while(!feof(temp))
 	{
       if (fgets(buff, 4096, temp) == NULL)
             break;
       string += buff;
 	}
	close(fdtemp);
	fclose(temp);
	client.setIsContentLen(string);
	client.setCgiBody(string);
}

static void	execution(std::string cgi, Client &client, int fdtemp, char **arg, char **env)
{
	int	fdToUse;

	if (client.getMethod() == POST)
	{
		if (client.getHeader().isChunkedEncoded())
		{
			rewind(client.getTmpChunked());
			fdToUse = client.getFdTmpChunked();
		}
		else
		{
			fseek(client.getTmp(), client.getHeaderSize(), SEEK_SET);
			fdToUse = client.getFdTmp();	
		}
		if (dup2(fdToUse, STDIN_FILENO) < 0)
		{
			perror("dup2()");
			exit(EXIT_FAILURE);
		}
	}
	if (dup2(fdtemp, STDOUT_FILENO) < 0)
	{
		perror("dup2()");
		exit(EXIT_FAILURE);
	}
	execve((char *)cgi.c_str(), arg, env);
	perror("execve()");
	exit(EXIT_FAILURE);
	exit(0);
}

int 		executeCGI(std::string cgi, Client &client)
{
	char		**env;
	char        *arg[2] = {(char *)cgi.c_str(), NULL};
	pid_t		pid;
	int			status;
	FILE		*temp = std::tmpfile();
	int			fdtemp = fileno(temp);
	int			timeoutTime = 1;
	
	if (mallocEnv(&env, client) < 0)
		return (ERR);
	
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		pid_t workerPid = fork();
		if (workerPid == 0)
			execution(cgi, client, fdtemp, arg, env);

    	pid_t	timeoutPid = fork();
		if (timeoutPid == 0)
		{
			sleep(timeoutTime);
			exit(0);
    	}

		pid_t exitedPid = wait(NULL);
		if (exitedPid == workerPid)
			kill(timeoutPid, SIGKILL);
		else
		{
			kill(workerPid, SIGKILL);
			write(fdtemp, "Content-type: text/plain\r\n\r\nTIMEOUT", 35);
		}
    	wait(NULL);
    	exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		setCgiString(temp, fdtemp, client);
		freeEnv(&env, client);
	}
	
	return (OK);
}
