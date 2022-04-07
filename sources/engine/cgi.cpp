/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 14:43:44 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/06 15:34:57 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

void	GetCGIfile(std::string *file, int *tot_size, char **env)
{
	int		fd, status;
	int		ret;
	char	buf[B_SIZE + 1];
	pid_t	pid;
	char	*arg[2];
	
	arg[0] = "/home/user42/Documents/42/webserv/bin-cgi/script.sh";
	arg[1] = NULL;

	std::cout << "here we are" << std::endl;
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		execve(arg[0], arg, env);
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
	
	fd = open("./bin-cgi/fileCGI", O_RDWR);
	std::cout << "fd = " << fd << std::endl;
	while ((ret = read(fd, buf, B_SIZE)) > 0)
	{
		(*file) += buf;
		(*tot_size) += ret;
	}
}