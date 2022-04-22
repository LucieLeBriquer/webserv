/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/22 15:09:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

int	main(int argc, char **argv, char **envp)
{
	Config		config(fileName(argc, argv));
	Socket		sock;

	if (!config.wellFormatted())
		return (1);

	sock = Socket(config);
	if (sock.getCheck() < 0)
		exit(EXIT_FAILURE);
	
	for (size_t i = 0; envp[i]; i++)
		sock.setEnv(envp[i]);
	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}
