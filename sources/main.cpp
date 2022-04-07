/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/06 23:20:01 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"


// just to test some URL

static void	testUrl(const Socket &socket, std::string url)
{
	Server	config;
	int		nbConfig;

	for (int i = 0; i < socket.getSocketNbr(); i++)
	{
		config = socket.getConfig(i);
		std::cout << socket.getConfig(i).getRoot() << "  [" << config.getRoot() << "]" << std::endl;
		std::cout << url << " requested on socket " << i << std::endl;
		nbConfig = config.configFromUrl(url);
		if (nbConfig >= 0)
			std::cout << socket.getConfig(i, nbConfig) << std::endl;
		else
			std::cout << "not in a location block, please use server's config" << std::endl;
		std::cout << config.getRealUrl(url) << std::endl << std::endl;
	}
} 

int	main(int argc, char **argv)
{
	Config		config(fileName(argc, argv));
	Socket		sock;

	if (!config.wellFormatted())
		return (1);

	sock = Socket(config);
	if (sock.getCheck() < 0)
		exit(EXIT_FAILURE);

	testUrl(sock, "/DIRECTORY4/blbl.html");

	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}
