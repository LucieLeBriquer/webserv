/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/25 17:30:01 by lpascrea         ###   ########.fr       */
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
	std::string			cgiCorps = client.getCgiCoprs();
	std::string			tmp;
	std::stringstream	out;
	int			i = 0;
	size_t		find;

	cgiHeader = deletingUseless(header);
	find = cgiCorps.find("Content-type:");
	if (find != std::string::npos)
	{
		while (cgiCorps[i + 3] && (cgiCorps[i] != '\r' && cgiCorps[i + 1] != '\n' && cgiCorps[i + 2] != '\r' && cgiCorps[i + 3] != '\n'))
			i++;
		tmp = &cgiCorps[i + 6];
		cgiCorps.erase(i + 4, strlen(cgiCorps.c_str()) - (i + 4));
	}
	else
		tmp = cgiCorps;
	client.setCgiCoprs(tmp);
	out << client.getCgiCoprs().length();
	cgiHeader += "Content-Length: ";
	cgiHeader += out.str();
	cgiHeader += "\r\n";
	cgiHeader += cgiCorps;
	cgiHeader += "\r\n";
	client.setIsContentLen(cgiHeader);
	return (cgiHeader);
}

static void	setCgiString(FILE *temp, int fdtemp, Client &client)
{
	char buff[4096];
	std::string string;

	std::rewind(temp);
	while(!feof(temp))
 	{
       if (fgets(buff, 4096, temp) == NULL)
             break;
       string += buff;
 	}
	close(fdtemp);
	std::fclose(temp);
	client.setIsContentLen(string);
	client.setCgiCoprs(string);
}

int 		getCGIfile(std::string cgi, Client &client)
{
	char		**env;
	char        *arg[2] = {(char *)cgi.c_str(), NULL};
	pid_t		pid;
	int			status;
	FILE		*temp = std::tmpfile();
	int			fdtemp = fileno(temp);
	int			timeout_sleep = 1;
	
	if (mallocEnv(&env, client) < 0)
		return (ERR);
	
	std::fseek(client.getTmp(), client.getHeaderSize(), SEEK_SET);
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		pid_t worker_pid = fork();
		if (worker_pid == 0)
		{
			if (client.getMethod() == POST)
			{			
				if (dup2(client.getFdTmp(), STDIN_FILENO) < 0)
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
			_exit(0);
		}

    	pid_t	timeout_pid = fork();
		if (timeout_pid == 0)
		{
			sleep(timeout_sleep);
			_exit(0);
    	}

		pid_t exited_pid = wait(NULL);
		if (exited_pid == worker_pid)
			kill(timeout_pid, SIGKILL);
		else
		{
			kill(worker_pid, SIGKILL);
			write(fdtemp, "Content-type: text/plain\r\n\r\nTIMEOUT", 35);
		}
    	wait(NULL);
    	_exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		setCgiString(temp, fdtemp, client);
		freeEnv(&env, client);
	}
	
	return (OK);
}
