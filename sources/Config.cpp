/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:02 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/29 15:17:38 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
**		USEFUL FUNCTIONS
*/

static bool	uselessLine(std::string line)
{
	int			i = 0;
	const char	*str = line.c_str();

	while (str[i] && isspace(str[i]))
		i++;
	return (!str[i] || str[i] == '#');
}

static std::string removeSpaces(std::string line)
{
	int			i = 0;
	int			j = line.length() - 1;
	const char	*str = line.c_str();

	while (str[i] && isspace(str[i]))
		i++;
	while (j > 0 && isspace(str[j]))
		j--;
	return (line.substr(i, j - i + 1));
}

static std::string removeCommentary(std::string line)
{
	size_t	commentPos;

	commentPos = line.find("#");
 	if (commentPos != std::string::npos)
		return (removeSpaces(line.substr(0, commentPos)) + "\n");
	return (removeSpaces(line) + "\n");
}

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Config::Config(void) : _formatOk(true)
{
	return ;
}

Config::Config(const Config &config) : _formatOk(true)
{
	*this = config;
}

static bool	readFile(const std::string file, std::string &fileString)
{
	std::ifstream	fileStream(file.c_str());
	std::string		line = "";

	if (!fileStream.is_open())
		return (false);
	while(std::getline(fileStream, line))
	{
		if (!uselessLine(line))
			fileString += removeCommentary(line);
	}
	fileStream.close();
	return (true);
}

Config::Config(const std::string file) : _formatOk(true)
{
	std::string 	fileString = "";
	vecStr 			serverBlocks;
	std::string		configInfo = "";

	if (readFile(file, fileString))
	{
		if (!splitBlocks(serverBlocks, fileString, "server", configInfo))
		{
			_formatOk = false;
			printFormatError("matching { } issues in a server block");
			return ;
		}
		if (configInfo.length() > 0)
		{
			_formatOk = false;
			printFormatError("not only server blocks");
			return ;
		}
		
		for (int i = 0; i < serverBlocks.size(); i++)
		{
			Server	newServ(serverBlocks[i]);

			if (!newServ.wellFormatted())
			{
				_formatOk = false;
				printFormatError(newServ.getFormatErr());
				return ;
			}
			_servers.push_back(newServ);
		}
	}
	else
	{
		_formatOk = false;
		printFileError(file);
	}
}

Config::~Config()
{
	_servers.clear();
}

/*
**		OVERLOAD OPERATORS
*/

Config	&Config::operator=(const Config &config)
{
	if (this != &config)
	{
		_servers.clear();
		
		_servers = config._servers;
		_formatOk = config._formatOk;
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Config &config)
{
	vecSrv	servers = config.getServers();
	
	o << std::endl << ORANGE << "Configuration" << END << std::endl << std::endl;
	for (int i = 0; i < servers.size(); i++)
		o << servers[i] << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

vecSrv	Config::getServers(void) const
{
	return (_servers);
}

bool	Config::wellFormatted(void) const
{
	return (_formatOk);
}
