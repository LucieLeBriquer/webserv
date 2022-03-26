/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:07 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 14:54:07 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	o << GREEN << "\tLocation" << END << std::endl << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Location::wellFormatted(void) const
{
	return (_formatOk);
}
