/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 13:32:53 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/19 14:31:46 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usefull.hpp"

static std::string	getType(std::string str)
{
	return (str.substr(0, str.find('/')));
}

static std::string	getSubtype(std::string str)
{
	return (str.substr(str.find('/') + 1, str.find(';')));
}


static std::string	removeQuality(std::string str)
{
	return (str.substr(0, str.find(';')));
}


std::string	mimeContentType(std::string accepted, std::string file)
{
	vecStr		splitted;
	vecStr		accept;
	std::string	extension;
	std::string	mime = "";
	std::string	type;
	std::string	subtype;
	std::string	mimeA;
	std::string	typeA;
	std::string	subtypeA;

	splitPattern(splitted, file, ".");
	splitPattern(accept, accepted, ",");

	if (accept.empty())
		return ("text/plain");
	if (splitted.empty())
		return (removeQuality(accept[0]));
	extension = splitted[splitted.size() - 1];
	for (int i = 0; i < nbMime; i++)
	{
		if (mimeExt[i] == extension)
		{
			mime = mimeType[i];
			break;
		}
	}
	if (mime.empty())
		return (removeQuality(accept[0]));
	type = getType(mime);
	subtype = getSubtype(mime);
	for (size_t i = 0; i < accept.size(); i++)
	{
		mimeA = accept[i];
		typeA = getType(mimeA);
		subtypeA = getSubtype(mimeA);
		if ((typeA == "*" && subtypeA == "*")
			|| (typeA == type && subtypeA == "*")
			|| (typeA == type && subtypeA == subtype))
			return (mime);
	}
	return (removeQuality(accept[0]));
}