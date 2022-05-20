/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 13:32:53 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/28 15:04:02 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "usefull.hpp"

static std::string	getType(std::string str)
{
	return (str.substr(0, str.find('/')));
}

static std::string	getSubtype(std::string str)
{
	size_t	quality = str.find(';');
	size_t	start = str.find('/');

	if (quality == std::string::npos)
		return (str.substr(start + 1, str.size() - start));
	else
		return (str.substr(start + 1, quality - start - 1));
}

static std::string	removeQuality(std::string str)
{
	size_t	quality = str.find(';');

	if (quality == std::string::npos)
		return (str);
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
	
	if (splitted.size() == 1 && accepted.size() > 1)
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
	if (mime.size() == 0)
		return ("text/plain");
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
	if (accepted.size() > 1)
		return (removeQuality(accept[0]));
	return (mime);
}