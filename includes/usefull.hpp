/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:23 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/08 11:43:16 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USEFULL_HPP
# define USEFULL_HPP
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
# include <iostream>
# include <vector>
# include <string>
# include <string.h>
# include <stdio.h>
# include <cstdlib>
# include <map>
# include <limits>
# include <sstream>
# include <arpa/inet.h>
#include <time.h>

#include "httpRequest.hpp"
#include "httpResponse.hpp"

typedef std::vector<std::string>	vecStr;
typedef std::map<int, std::string>	mapErr;
typedef	std::vector<int>			vecInt;

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

template<typename T> std::ostream	&operator<<(std::ostream &o, std::vector<T> vect)
{
	for (size_t i = 0; i + 1 < vect.size(); i++)
		o << vect[i] << " ";
	if (vect.size() > 0)
		o << vect[vect.size() - 1];
	return (o);
}

std::ostream	&operator<<(std::ostream &o, mapErr map);

static const int			nbMethods = 3;
static const std::string	methods[nbMethods] = {"GET", "POST", "DELETE"};
std::string    fileName(int argc, char **argv);

std::string 				copystr(std::string str, int start);
std::vector<std::string>	splitThis(std::string str);

#endif
