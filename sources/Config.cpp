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

Config::Config(void)
{
	return ;
}

Config::Config(const Config &config)
{
	*this = config;
}


// parsing
Config::Config(std::string file)
{
	std::cout << YELLOW << "[Config] constructor" << END << std::endl;

	std::ifstream	fileStream(file.c_str());
	std::string 	fileString = "";
	std::string		line = "";
	vecStr 			serverBlocks;
	std::string		configInfo;

	if (fileStream.is_open())
	{
    	while(std::getline(fileStream, line))
		{
			if (!uselessLine(line))
				fileString += removeCommentary(line);
		}
		fileStream.close();

		configInfo.clear();
		Location::splitBlocks(serverBlocks, fileString, "server", configInfo);
		std::cout << BLUE << "configInfo" << END << "--->" << configInfo << "<---" << std::endl;
		for (int i = 0; i < serverBlocks.size(); i++)
		{
			Server	newServ(serverBlocks[i]);

			if (!newServ.wellFormatted())
			{
				Location::printFormatError();
				return ;
			}
			_servers.push_back(newServ);
		}
	}
	else
	{
		std::cerr << "Error: can't open configuration file " << file << std::endl;
	}
}

Config::~Config()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

Config	&Config::operator=(const Config &config)
{
	if (this != &config)
	{
		/* code */
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Config &config)
{
	(void)config;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/
