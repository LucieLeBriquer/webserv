/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/31 11:16:23 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Config.hpp"
#include "engine.hpp"

int	main(/*int argc, char **argv*/void)
{
	// Config	config(fileName(argc, argv));
	Socket		sock;

	// if (!config.wellFormatted())
	// 	return (1);
	// std::cout << config << std::endl;
	
	/******instead of config lucie******/
	std::map<int, const char *> map;
	std::pair<int, const char *> conf1 = std::make_pair(8080, "127.0.0.1");
	std::pair<int, const char *> conf2 = std::make_pair(8081, "127.0.0.2");
	std::pair<int, const char *> conf3 = std::make_pair(8082, "127.0.0.1");
	map.insert(conf1);
	map.insert(conf2);
	map.insert(conf3);
	/***********************************/

	if (!init_sockets(&sock, map))
		exit(EXIT_FAILURE);
	if (!init_epoll(&sock, map))
		exit(EXIT_FAILURE);
	
	return (0);
}
