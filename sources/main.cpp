/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/15 08:59:37 by user42           ###   ########.fr       */
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
	for (int i = 0; envp[i]; i++)
		sock.setEnv(envp[i]);
	if (sock.getCheck() < 0)
		exit(EXIT_FAILURE);

	if (!initEpoll(&sock, config))
		exit(EXIT_FAILURE);
	return (0);
}
