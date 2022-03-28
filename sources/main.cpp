/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 14:47:53 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static std::string	fileName(int argc, char **argv)
{
	std::string		configFile = "config/basic.conf";

	if (argc >= 2)
		configFile = argv[1];
	return (configFile);
}

int	main(int argc, char **argv)
{
	Config	config(fileName(argc, argv));

	if (!config.wellFormatted())
		return (-1);
	std::cout << config << std::endl;
	return (0);
}
