/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 16:05:50 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/29 14:28:58 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"


Block::Block(void) : _root("/"), _maxClientBodySize(1), _redirUrl(""), _autoindex(false),
					_rootSet(false), _indexSet(false), _maxClientBodySizeSet(false), _methodsSet(false), 
					_errorPagesSet(false), _redirUrlSet(false), _autoindexSet(false), 
					_formatOk(true), _formatErr("")
{
	return ;
}

Block::Block(const Block &block)
{
	*this = block;
}

Block::~Block()
{
	_index.clear();
	_methods.clear();
	_errorPages.clear();
}

/*
**		OVERLOAD OPERATORS
*/

Block	&Block::operator=(const Block &block)
{
	if (this != &block)
	{
		_index.clear();
		_methods.clear();
		_errorPages.clear();

		_root = block._root;
		_index = block._index;
		_maxClientBodySize = block._maxClientBodySize;
		_methods = block._methods;
		_errorPages = block._errorPages;
		_redirUrl = block._redirUrl;
		_autoindex = block._autoindex;

		_rootSet = block._rootSet;
		_indexSet = block._indexSet;
		_maxClientBodySizeSet = block._maxClientBodySizeSet;
		_methodsSet = block._methodsSet;
		_errorPagesSet = block._errorPagesSet;
		_redirUrlSet = block._redirUrlSet;
		_autoindexSet = block._autoindexSet;

		_formatOk = block._formatOk;
		_formatErr = block._formatErr;
	}
	return (*this);
}

/*
**		MEMBER FUNCTIONS
*/

bool	Block::wellFormatted(void) const
{
	return (_formatOk);
}

void	Block::_setWrongFormat(void)
{
	_formatOk = false;
}

void	Block::_setWrongFormat(std::string str)
{
	_formatOk = false;
	_formatErr = str;
}

/*
**		SETTER FUNCTIONS
*/

void	Block::_setRoot(vecStr words)
{
	if (_rootSet)
		return (_setWrongFormat("root already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need \"root PATH\""));
	_root = words[1];
	_rootSet = true;
}

void	Block::_setIndex(vecStr words)
{
	if (_indexSet)
		return (_setWrongFormat("index already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one index page"));
	for (size_t i = 1; i < words.size(); i++)
		_index.push_back(words[i]);
	_indexSet = true;
}

void	Block::_setMaxClientBody(vecStr words) // to fix negative value
{
	if (_maxClientBodySizeSet)
		return (_setWrongFormat("client_size already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need client_size SIZE"));
	_maxClientBodySize = myAtoi(words[1]);
	if (_maxClientBodySize < 0)
		return (_setWrongFormat("size for client_size not well formatted"));
	if (_maxClientBodySize == 0)
		_maxClientBodySize = std::numeric_limits<size_t>::max();
	_maxClientBodySizeSet = true;
}

void	Block::_setMethods(vecStr words)
{
	if (_methodsSet)
		return (_setWrongFormat("methods already defined"));
	if (words.size() < 2)
		return (_setWrongFormat("need at least one method"));
	for (size_t i = 1; i < words.size(); i++)
	{
		int method = getMethodNb(words[i]);
		if (method < 0)
			return (_setWrongFormat(words[i] + " is an unkown method"));
		_methods.push_back(method);
	}
	_methodsSet = true;
}

void	Block::_setErrorPages(vecStr words)
{
	std::pair<int, std::string>	err;
	
	if (words.size() != 3)
		return (_setWrongFormat("need error ERROR_NUM ERROR_PAGE"));
	err.first = myAtoi(words[1]); // to fix
	if (err.first < 0)
		return (_setWrongFormat("unvalid ERROR_NUM"));
	err.second = words[2];
	_errorPages.insert(err);
	_errorPagesSet = true;
}

void	Block::_setAutoIndex(vecStr words)
{
	if (_autoindexSet)
		return (_setWrongFormat("error_pages already defined"));
	if (words.size() != 1)
		return (_setWrongFormat("autoindex doesn't take arguments"));
	_autoindex = true;
	_autoindexSet = true;
}

void	Block::_setRedirUrl(vecStr words)
{
	if (_redirUrlSet)
		return (_setWrongFormat("return already defined"));
	if (words.size() != 2)
		return (_setWrongFormat("need return REDIR_URL"));
	_redirUrl = words[1];
	_redirUrlSet = true;
}

/*
**		GETTER FUNCTIONS
*/

std::string	Block::getRoot(void) const
{
	return (_root);
}

vecStr		Block::getIndex(void) const
{
	return (_index);
}

size_t		Block::getMaxClientBody(void) const
{
	return (_maxClientBodySize);
}

vecInt		Block::getMethods(void) const
{
	return (_methods);
}

mapErr		Block::getErrorPages(void) const
{
	return (_errorPages);
}

std::string	Block::getRedirUrl(void) const
{
	return (_redirUrl);
}

bool		Block::getAutoIndex(void) const
{
	return (_autoindex);
}

std::string	Block::getFormatErr(void) const
{
	return (_formatErr);
}
