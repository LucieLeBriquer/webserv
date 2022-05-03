/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/27 14:51:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int	main(int argc, char **argv)
{
	Config		config(fileName(argc, argv));
	Socket		sock;

	if (!config.wellFormatted())
		return (1);

	sock = Socket(config);
	if (sock.getCheck() < 0)
		exit(EXIT_FAILURE);
	
	sock.setCgiCoprs("");
	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}