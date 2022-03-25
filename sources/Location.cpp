#include "Location.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/
/*
	std::string			_root;
		std::string			_index;
		size_t				_maxClientBody;
		std::vector<int>	_methods;
		bool				_autoindex;
		bool				_formatOk;
*/
Location::Location(void) : _root(""), _index(""), _maxClientBody(0), _autoindex(false), _formatOk(true)
{
	return ;
}

Location::Location(const Location &location)
{
	*this = location;
}

Location::Location(std::string fileString, size_t start)
{
	return ;
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