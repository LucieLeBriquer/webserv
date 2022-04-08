/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:42:38 by masboula          #+#    #+#             */
/*   Updated: 2022/04/08 11:42:38 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usefull.hpp"

HTTPRequest::HTTPRequest()
{
};

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

std::string	HTTPRequest::getFirstLine( void )
{
	this->_fLine = this->_method + " " + this->_url + " " + this->_httpv;
	std::cout << "fline = "<< _fLine << std::endl;
	return this->_fLine;
}
