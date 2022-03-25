#include "Config.hpp"

/*
**		USEFUL FUNCTIONS
*/



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
	std::ifstream	fileStream(file.c_str());
	std::string 	fileString;
	std::string		line;
	size_t			sBlockPos;

	if (fileStream.is_open())
	{
    	while(std::getline(fileStream, line))
		{
			if (!uselessLine(line))
				fileString += line + "\n";
		}
		fileStream.close();
		//std::cout << fileString;
		sBlockPos = fileString.find("server {");
 		while (sBlockPos != std::string::npos)
		{
			Server	newServ(fileString, sBlockPos);
			if (!newServ.wellFormatted())
				return ;
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
