#include "Config.hpp"

/*
**		USEFUL FUNCTIONS
*/



static bool	uselessLine(std::string line)
{
	int		i = 0;

	const char *str = line.c_str();

	while (str[i] && isspace(str[i]))
		i++;
	return (!str[i] || str[i] == '#');
}

static std::string removeCommentary(std::string line)
{
	size_t	commentPos;

	commentPos = line.find("#");
 	if (commentPos != std::string::npos)
		return (line.substr(0, commentPos) + "\n");
	return (line + "\n");
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

void	showFileString(std::string fileString)
{
	std::cout << "====================================================" << std::endl;
	std::cout << fileString;
	std::cout << "====================================================" << std::endl;
}
// parsing
Config::Config(std::string file)
{
	std::ifstream	fileStream(file.c_str());
	std::string 	fileString = "";
	std::string		line = "";
	size_t			sBlockPos = 0;

	if (fileStream.is_open())
	{
    	while(std::getline(fileStream, line))
		{
			if (!uselessLine(line))
				fileString += removeCommentary(line);
		}
		fileStream.close();
		showFileString(fileString);
		sBlockPos = fileString.find("server {", 0);
 		while (sBlockPos != std::string::npos)
		{
			Server	newServ(fileString, sBlockPos);
			if (!newServ.wellFormatted())
				return ;
			_servers.push_back(newServ);
			std::cout << "new server added " << sBlockPos << std::endl;
			sBlockPos = fileString.find("server {", sBlockPos + 8);
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
