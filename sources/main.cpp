/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/31 14:44:14 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int	main(int argc, char **argv)
{
	Config		config(fileName(argc, argv));
	Socket		sock(config);

	if (!config.wellFormatted())
		return (1);
	//std::cout << config << std::endl;
	
	// if (!initSockets(&sock, config))
		// exit(EXIT_FAILURE);
	if (sock.getCheck() < 0)
		exit(EXIT_FAILURE);
	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}
