/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/18 16:41:39 by lle-briq         ###   ########.fr       */
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

	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}
