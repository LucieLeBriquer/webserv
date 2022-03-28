/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:23 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 17:57:41 by lle-briq         ###   ########.fr       */
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
# include <iostream>
# include <vector>
# include <string>
# include <cstdlib>
# include <map>
# include <limits>

typedef std::vector<std::string>	vecStr;
typedef std::map<int, std::string>	mapErr;
typedef	std::vector<int>			vecInt;

bool	splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo);
void	splitPattern(vecStr &splitted, std::string str, std::string pattern);

void	printFormatError(void);
void	printFormatError(std::string);
void	printFileError(std::string file);

bool	checkHostFormat(std::string str);
int		myAtoi(std::string str);

int				getMethodNb(std::string method);
std::string		getMethod(int methodNm);
std::ostream	&showMethod(std::ostream &o, vecInt methods);

template<typename T> std::ostream	&operator<<(std::ostream &o, std::vector<T> vect)
{
	for (int i = 0; i + 1 < vect.size(); i++)
		o << vect[i] << " ";
	if (vect.size() > 0)
		o << vect[vect.size() - 1];
	return (o);
}

std::ostream	&operator<<(std::ostream &o, mapErr map);

static const int			nbMethods = 3;
static const std::string	methods[nbMethods] = {"GET", "POST", "DELETE"};

#endif
