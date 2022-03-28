/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:07 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 23:58:16 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/*
**		CONSTRUCTORS AND DESTRUCTOR
*/

Location::Location(void) : Block(), _path("/"), _cgiPass(""), _cgiPassSet(false)
{
	return ;
}

Location::Location(const Location &location) : Block(location)
{
	*this = location;
}

Location::Location(std::string str) : Block(), _path("/"), _cgiPass(""), _cgiPassSet(false)
{
	vecStr	lines;

	splitPattern(lines, str, "\n");
	if (!_setPath(lines[0]))
	{
		_setWrongFormat("wrong start of location block");
		return ;
	}
	for (int i = 1; i + 1 < lines.size(); i++)
	{
		_fillOneInfo(lines[i]);
		if (!_formatOk)
			return ;
	}
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
		(Block)(*this) = (Block)(location);
		_path = location._path;
		_cgiPass = location._cgiPass;
		_cgiPassSet = location._cgiPassSet;
	}
	return (*this);
}

std::ostream	&operator<<(std::ostream &o, const Location &location)
{
	o << GREEN << "\n\tLocation" << END << "\t" << location.getPath() << std::endl;
	o << "\t\troot        \t" << location.getRoot() << std::endl;
	o << "\t\tindex       \t" << location.getIndex() << std::endl;
	o << "\t\tclient_size \t" << location.getMaxClientBody() << std::endl;
	o << "\t\tmethods     \t";
	showMethod(o, location.getMethods()) << std::endl;
	o << "\t\terror_pages \t" << location.getErrorPages() << std::endl;
	o << "\t\treturn      \t" << location.getRedirUrl() << std::endl;
	o << "\t\tautoindex   \t" << ((location.getAutoIndex() == 0) ? "off" : "on") << std::endl;
	o << "\t\tcgi_pass    \t" << location.getCgiPass() << std::endl;
	return (o);
};

/*
**		MEMBER FUNCTIONS
*/

bool	Location::_setPath(std::string str)
{
	vecStr	words;

	splitPattern(words, str, " ");
	if (words.size() != 3)
		return (false);
	_path = words[1];
	return (true);
}

void	Location::_setCgiPass(vecStr words)
{
	if (_cgiPassSet)
		return (_setWrongFormat("cgi_pass already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need cgi_pass CGI"));
	_cgiPass = words[1];
	_cgiPassSet = true;
}

void	Location::_fillOneInfo(std::string str)
{
	int		keyword;
	vecStr	words;
	setFunc3	setters[nbKeywords] = {&Location::_setRoot, &Location::_setIndex, &Location::_setMaxClientBody,
			&Location::_setMethods, &Location::_setErrorPages, &Location::_setAutoIndex,
			&Location::_setRedirUrl, &Location::_setCgiPass};

	splitPattern(words, str, " ");
	keyword = _keywordNumber(words[0]);
	if (keyword < 0)
	{
		_formatOk = false;
		_formatErr = "unknown keyword \"" + words[0] + "\"";
		return ;
	}
	return ((this->*(setters[keyword]))(words));
}


int		Location::_keywordNumber(std::string str)
{
	for (int i = 0; i < nbKeywords; i++)
	{
		if (str == keywords[i])
			return (i);
	}
	_formatOk = false;
	return (-1);
}

std::string	Location::getPath(void) const
{
	return (_path);
}

std::string	Location::getCgiPass(void) const
{
	return (_cgiPass);
}

std::string Location::keywords[nbKeywords] = {"root", "index", "client_size", "methods", "error_page", "autoindex", "return", "cgi_pass"};
