/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:15 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 14:41:51 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usefull.hpp"

static size_t	endOfBlock(std::string str, size_t start)
{
	int		needed = 1;
	size_t	bracketOpen;
	size_t	bracketClose;
	size_t	lastBracket;

	bracketOpen = str.find("{", start);
	if (bracketOpen == std::string::npos)
		return (std::string::npos);
	lastBracket = bracketOpen;
	while (needed > 0 && lastBracket != std::string::npos)
	{
		bracketOpen = str.find("{\n", lastBracket + 1);
		bracketClose = str.find("\n}\n", lastBracket + 1) + 1;
		if (bracketClose == std::string::npos && bracketOpen == std::string::npos)
			return (std::string::npos);
		if (bracketOpen < bracketClose)
		{
			needed++;
			lastBracket = bracketOpen;
		}
		else
		{
			needed--;
			lastBracket = bracketClose;
		}
	}
	return (lastBracket);
}

static std::string	substrPos(std::string str, size_t start, size_t end)
{
	if (end + 1 > start)
		return (str.substr(start, end - start + 1));
	return ("");
}

static size_t	newStartOfBlock(std::string str, std::string pattern, size_t start)
{
	size_t	pos;
	size_t	brackOpen;

	pos = str.find(pattern, start);
	while (pos != std::string::npos)
	{
		if (!isspace(str.c_str()[pos + pattern.length()]))
			return (std::string::npos);
		brackOpen = str.find("{\n");
		if (brackOpen != std::string::npos && str.find("\n", pos) > brackOpen)
			return (pos);
	}
	return (std::string::npos);
}

void splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo)
{
	size_t	pos;
	size_t	end = 0;

	pos = newStartOfBlock(str, pattern, 0);
	while (pos != std::string::npos)
	{
		otherInfo += substrPos(str, end == 0 ? 0 : end + 2, pos - 1);
		end = endOfBlock(str, pos);
		if (end != std::string::npos)
			splitted.push_back(str.substr(pos, end - pos + 1));
		else
			return (printFormatError());
		pos = newStartOfBlock(str, pattern, end);
	}
	otherInfo += substrPos(str, end == 0 ? 0 : end + 2, str.length() - 1);
}

static std::string	removeInsideSpaces(std::string str)
{
	std::string res;
	size_t		i = 0;		

	res.clear();
	while (i < str.length())
	{
		if (isspace(str.c_str()[i]))
		{
			while (i < str.length() && isspace(str.c_str()[i]))
				i++;
			res += " ";
		}
		if (i < str.length())
			res += str.c_str()[i];
		i++;
	}	
	return (res);
}

void	splitPattern(vecStr &splitted, std::string str, std::string pattern)
{
	size_t		pos;
	size_t		prevPos = 0;
	std::string	toAdd;

	pos = str.find(pattern, 0);
	while (pos != std::string::npos)
	{
		toAdd = substrPos(str, prevPos == 0 ? 0 : prevPos + pattern.length(), pos - 1);
		splitted.push_back(removeInsideSpaces(toAdd));
		prevPos = pos;
		pos = str.find(pattern, pos + pattern.length());
	}
	toAdd = substrPos(str, prevPos == 0 ? 0 : prevPos + pattern.length(), str.length() - 1);
	splitted.push_back(removeInsideSpaces(toAdd));
}

void	printFormatError(void)
{
	std::cerr << "Error: wrong format in configuration file" << std::endl;
}

void	printFormatError(std::string str)
{
	std::cerr << "Error: wrong format in configuration file " << GRAY << "(" << str << ")" << END << std::endl;
}

void	printFileError(std::string file)
{
	std::cerr << "Error: can't open configuration file " << file << std::endl;
}

int		myAtoi(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.c_str()[i]))
			return (-1);
	}
	return (atoi(str.c_str()));
}

bool	checkHostFormat(std::string str)
{
	vecStr	addr;
	int		n;

	splitPattern(addr, str, ".");
	if (addr.size() != 4)
		return (false);
	for (int i = 0; i < addr.size(); i++)
	{
		n = myAtoi(addr[i]);
		if (n < 0 || n > 255)
			return (false);
	}
	return (true);
}

std::ostream	&operator<<(std::ostream &o, mapErr map)
{
	for (int i = 0; i + 1 < map.size(); i++)
		o << "(" << map[i][0] << "," << map[i][1] << ") ";
	if (map.size() > 0)
		o << "(" << map[map.size() - 1][0] << "," << map[map.size() - 1][1] << ")";
	return (o);
}

int	getMethodNb(std::string method)
{
	for (int i = 0; i < nbMethods; i++)
	{
		if (methods[i] == method)
			return (i);
	}
	return (-1);
}

std::string		getMethod(int methodNb)
{
	if (methodNb >= 0 && methodNb < nbMethods)
		return (methods[methodNb]);
	return ("unknown");
}

std::ostream	&showMethod(std::ostream &o, vecInt methods)
{
	for (int i = 0; i + 1 < methods.size(); i++)
		o << getMethod(methods[i]) << " ";
	if (methods.size() > 0)
		o << getMethod(methods[methods.size() - 1]);
	return (o);
}
