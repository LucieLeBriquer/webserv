/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:23 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/19 16:50:11 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USEFULL_HPP
# define USEFULL_HPP
# include "mime.hpp"
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define ORANGE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define BLUE "\x1B[36m"
# define BOLD "\x1B[1m"
# define GRAY "\x1B[90m"
# define END "\x1B[0m"
# define ERR -1
# define OK 0
# define LOG 0
# define DIRECTORY 0
# define REGFILE 1

// librairies
# include <iostream>
# include <sstream>
# include <fstream>

# include <vector>
# include <map>
# include <string>
# include <algorithm>

# include <limits>
# include <utility>

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <errno.h>

# include <cstdlib>
# include <cstdio>

# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>

typedef std::vector<std::string>					vecStr;
typedef std::map<std::string, std::string>			mapStr;
typedef std::map<int, std::string>					mapErr;
typedef	std::vector<int>							vecInt;
typedef	std::map<int, int>							mapSock;
typedef std::vector< std::pair<std::string, int> >	vecFiles;
typedef std::pair<std::string, std::string> 		pairStr;
typedef struct dirent fileInfo;
typedef DIR	directory;

bool	splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo);
void	splitPattern(vecStr &splitted, std::string str, std::string pattern);

void	printFormatError(void);
void	printFormatError(std::string);
void	printFileError(std::string file);

bool	checkHostFormat(std::string str);
bool	checkWordFormat(std::string str);

size_t	myAtoi(std::string str);
size_t	myAtoi(std::string str, bool &success);

int				getMethodNb(std::string method);
std::string		getMethod(int methodNm);
std::ostream	&showMethod(std::ostream &o, vecInt methods);

std::string		fileName(int argc, char **argv);
std::string 	copystr(std::string str, int start);
vecStr			splitThis(std::string str);
std::string		removeSlash(const std::string &str);
std::string		toString(int nb);
std::string		toString(size_t nb);
std::string		toString(long nb);

bool			isDirectory(std::string path);
bool			isRegFile(std::string path);

std::string		mimeContentType(std::string accepted, std::string extension);

template<typename T> std::ostream	&operator<<(std::ostream &o, std::vector<T> vect)
{
	for (size_t i = 0; i + 1 < vect.size(); i++)
		o << vect[i] << " ";
	if (vect.size() > 0)
		o << vect[vect.size() - 1];
	return (o);
}

std::ostream	&operator<<(std::ostream &o, mapErr map);

static const int			nbMethods = 5;
static const std::string	methods[nbMethods] = {"GET", "POST", "DELETE", "HEAD", "OPTIONS"};


// errors
static const int			nbError = 4;

static const std::string	errorNb[nbError] = {
	"400",
	"403",
	"404",
	"405"
};

static const std::string	errorMsg[nbError] = {
	"Bad Request",
	"Forbiden",
	"Page Not Found",
	"Method Not Allowed"
};

static const std::string	errorExplanation[nbError] = {
	"The server cannot process the request due to something<br>that is perceived to be a client error (e.g., malformed request syntax,<br>invalid request message framing, or deceptive request routing).",
	"You don't have permission to access<br>this resource.",
	"The page you are looking for might have been removed,<br>	had its name changed or is temporarily unavailable.",
	"The method received in the request-line is not<br>	supported by the target resource."
};

#endif
