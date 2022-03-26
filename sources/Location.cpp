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
	//std::cout << YELLOW << "[Location] constructor" << END << std::endl;
	//std::cout << "from\n--->" << str << "<---" << std::endl << std::endl;
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
		brackOpen = str.find("{\n");
		if (brackOpen != std::string::npos && str.find("\n", pos) > brackOpen)
			return (pos);
	}
	return (std::string::npos);
}

void Location::splitBlocks(std::vector<std::string> &splitted, std::string str, std::string pattern, std::string &otherInfo)
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

void	Location::splitLines(std::vector<std::string> &splitted, std::string str)
{
	size_t		pos;
	size_t		prevPos = 0;
	std::string	toAdd;

	pos = str.find("\n", 0);
	while (pos != std::string::npos)
	{
		toAdd = substrPos(str, prevPos == 0 ? 0 : prevPos + 1, pos - 1);
		splitted.push_back(removeInsideSpaces(toAdd));
		prevPos = pos;
		pos = str.find("\n", pos + 1);
	}
	toAdd = substrPos(str, prevPos == 0 ? 0 : prevPos + 1, str.length() - 1);
	splitted.push_back(removeInsideSpaces(toAdd));
}

void	Location::printFormatError(void)
{
	std::cerr << "Error: wrong format in configuration file" << std::endl;
}