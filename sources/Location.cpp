#include "Location.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Location::Location(void) : _root(""), _index(""), _maxClientBody(0), _redirUrl(""), _autoindex(false), _formatOk(true)
{
	return ;
}

Location::Location(const Location &location)
{
	*this = location;
}

Location::Location(std::string str) : _formatOk(true)
{
	std::cout << "[Location] constructor from :\n" << str << std::endl << std::endl;
}

Location::~Location()
{
	return ;
}

/*
**		OVERLOAD OPERATORS
*/

Location	&Location::operator=(const Location &location)
{
	if (this != &location)
	{
		_root = location._root;
		_index = location._index;
		_maxClientBody = location._maxClientBody;
		_methods = location._methods;
		_autoindex = location._autoindex;
		_formatOk = location._formatOk;
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Location &location)
{
	(void)location;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Location::wellFormatted(void) const
{
	return (_formatOk);
}

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
		bracketOpen = str.find("{", lastBracket + 1);
		bracketClose = str.find("}", lastBracket + 1);
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

void Location::splitBlocks(std::vector<std::string> &splitted, std::string str, std::string pattern, std::string &otherInfo)
{
	size_t	pos;
	size_t	end = 0;

	pos = str.find(pattern, 0);
	while (pos != std::string::npos)
	{
		otherInfo += str.substr(end == 0 ? 0 : end + 1, pos - end - 1);
		end = endOfBlock(str, pos); 
		if (end != std::string::npos)
			splitted.push_back(str.substr(pos, end - pos + 1));
		else
			std::cerr << "Error: wrong format in configuration file" << std::endl;
		pos = str.find(pattern, end);
	}
	otherInfo += str.substr(end, str.length() - end);
}