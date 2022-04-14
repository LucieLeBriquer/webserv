/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usefull.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:53:15 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/14 11:37:16 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usefull.hpp"

/*
**		SPLIT FUNCTIONS
*/

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
		bracketClose = str.find("\n}\n", lastBracket + 1);
		if (bracketClose == std::string::npos)
			return (std::string::npos);
		else
			bracketClose++;
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
		brackOpen = str.find("{\n");
		if (brackOpen != std::string::npos && str.find("\n", pos) > brackOpen)
			return (pos);
	}
	return (std::string::npos);
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

bool splitBlocks(vecStr &splitted, std::string str, std::string pattern, std::string &otherInfo)
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
			return (false);
		pos = newStartOfBlock(str, pattern, end);
	}
	otherInfo += substrPos(str, end == 0 ? 0 : end + 2, str.length() - 1);
	return (true);
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


/*
**		PRINT ERRORS AND HELP
*/

static void	printConfFormat(std::string str)
{
	if (str.find("{ }") == std::string::npos)
		return ;
	std::cerr << std::endl << YELLOW << "[Expected]" << END << std::endl;
	std::cerr << "server {" << std::endl;
	std::cerr << "\tlisten\t\t\tHOST:PORT" << std::endl;
	std::cerr << "\tserver_name\t\tNAME0 NAME1 [...]" << std::endl;
	std::cerr << "\troot\t\t\tROOT" << std::endl;
	std::cerr << "\tindex\t\t\tINDEX0 INDEX1 [...]" << std::endl;
	std::cerr << "\tclient_size\t\tSIZE" << std::endl;
	std::cerr << "\tmethods\t\t\tMETHOD0 METHOD1 [...]" << std::endl;
	std::cerr << "\treturn\t\t\tREDIR_URL" << std::endl;
	std::cerr << "\terror_page\t\tERROR_NUM0 ERROR_PAGE0" << std::endl;
	std::cerr << "\terror_page\t\tERROR_NUM1 ERROR_PAGE1" << std::endl;
	std::cerr << "\t\t\t\t\t[...]" << std::endl;
	std::cerr << "\tautoindex\t\ton" << std::endl;

	std::cerr << std::endl;
	std::cerr << "\tlocation PATH {" << std::endl;
	std::cerr << "\t\troot\t\t\tROOT" << std::endl;
	std::cerr << "\t\tindex\t\t\tINDEX0 INDEX1 [...]" << std::endl;
	std::cerr << "\t\tclient_size\t\tSIZE" << std::endl;
	std::cerr << "\t\tmethods\t\t\tMETHOD0 METHOD1 [...]" << std::endl;
	std::cerr << "\t\treturn\t\t\tREDIR_URL" << std::endl;
	std::cerr << "\t\terror_page\t\tERROR_NUM0 ERROR_PAGE0" << std::endl;
	std::cerr << "\t\terror_page\t\tERROR_NUM1 ERROR_PAGE1" << std::endl;
	std::cerr << "\t\t\t\t\t\t[...]" << std::endl;
	std::cerr << "\t\tautoindex\t\toff" << std::endl;
	std::cerr << "\t}" << std::endl;
	std::cerr << "}" << std::endl;
}

void	printFormatError(void)
{
	std::cerr << RED << "[Error]" << END << " wrong configuration file" << std::endl;
	printConfFormat("");
}

void	printFormatError(std::string str)
{
	std::cerr << RED << "[Error]" << END << " wrong configuration file ";
	std::cerr << GRAY << "(" << str << ")" << END << std::endl;
	printConfFormat(str);
}

void	printFileError(std::string file)
{
	std::cerr << RED << "[Error]" << END << " can't open configuration file " << GRAY << file << END << std::endl;
}


/*
**		ATOI
*/

size_t	myAtoi(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.c_str()[i]))
			return (-1);
	}
	return (atoi(str.c_str()));
}


size_t	myAtoi(std::string str, bool &succeed)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.c_str()[i]))
		{
			succeed = false;
			return (-1);
		}
	}
	succeed = true;
	return (atoi(str.c_str()));
}


/*
**		CHECK FORMAT
*/

bool	checkHostFormat(std::string str)
{
	vecStr	addr;
	size_t	n;
	bool	success;

	splitPattern(addr, str, ".");
	if (addr.size() != 4)
		return (false);
	for (size_t i = 0; i < addr.size(); i++)
	{
		n = myAtoi(addr[i], success);
		if (!success || n > 255)
			return (false);
	}
	return (true);
}

bool	checkWordFormat(std::string str)
{
	std::string validChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";

	for (size_t i = 0; i < str.size(); i++)
	{
		if (validChar.find(str[i], 0) == std::string::npos)
			return (false);
	}
	return (true);
}


/*
**		MAP PRINTING OVERLOAD
*/

std::ostream	&operator<<(std::ostream &o, mapErr map)
{
	mapErr::iterator	it = map.begin();
	while (it != map.end())
	{
		if (it == map.begin())
			o << "(" << it->first << " " << it->second << ")";
		else
			o << "\t\t\t\t\t(" << it->first << " " << it->second << ")";
		it++;
		if (it != map.end())
			o << std::endl;
	}
	return (o);
}


/*
**		METHODS FUNCTIONS
*/

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
	for (size_t i = 0; i + 1 < methods.size(); i++)
		o << getMethod(methods[i]) << " ";
	if (methods.size() > 0)
		o << getMethod(methods[methods.size() - 1]);
	return (o);
}


/*
**		GET FILE NAME
*/

std::string	fileName(int argc, char **argv)
{
	std::string		configFile = "config/basic.conf";

	if (argc >= 2)
		configFile = argv[1];
	return (configFile);
}


/*
**		URL MANIP FUNCTIONS
*/

std::string			removeSlash(const std::string &str)
{
	if (str.size() > 0 && str[0] == '/')
		return (str.substr(1, str.size() - 1));
	return (str);
}