/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:23 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 14:53:24 by lle-briq         ###   ########.fr       */
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
# define END "\x1B[0m"
# include <iostream>
# include <vector>
# include <string>
# include <cstdlib>
# include <map>

typedef std::vector<std::string>	vecStr;
typedef std::map<int, std::string>	mapErr;
typedef	std::vector<int>			vecInt;

void	splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo);
void	splitPattern(vecStr &splitted, std::string str, std::string pattern);

void	printFormatError(void);
void	printFileError(std::string file);

bool	checkHostFormat(std::string str);
int		myAtoi(std::string str);

#endif
