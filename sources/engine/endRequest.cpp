/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 15:10:02 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/13 16:53:45 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int		endRequest(Client &client)
{
	FILE		*file = client.getTmp();
	int 		lastChar[4];

	if (client.getMethod() == BAD_METHOD)
	{
		for (int i = 0; i < nbMethods; i++)
		{
			if (strncmp(client.getRequest().c_str(), methods[i].c_str(), methods[i].size()) == 0)
				client.setMethod(getMethodNb(methods[i]));
		}
	}

	if (!client.hasRecvHeader())
	{
		std::rewind(file);
		while (!std::feof(file))
		{
			std::fseek(file, -3, SEEK_CUR);
			lastChar[0] = std::fgetc(file);
			lastChar[1] = std::fgetc(file);
			lastChar[2] = std::fgetc(file);
			lastChar[3] = std::fgetc(file);

			if (lastChar[0] == '\r' && lastChar[1] == '\n'
				&& lastChar[2] == '\r' && lastChar[3] == '\n')
			{
				client.changeRecvHeader();
				if (client.getMethod() == POST)
					return (OK);
				break;
			}
		}
	}

	if (client.hasRecvHeader())
	{
		if (client.getMethod() != POST)
			return (ERR);
		else
		{
			std::fseek(file, -2, SEEK_END);
			lastChar[0] = std::fgetc(file);
			lastChar[1] = std::fgetc(file);
			if (lastChar[0] == '\r' && lastChar[1] == '\n')
				return (ERR);
		}
	}
	std::fseek(file, 0, SEEK_END);
	return (OK);
}
