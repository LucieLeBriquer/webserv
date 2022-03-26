/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:12 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 14:54:12 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int	main(int argc, char **argv)
{
	std::string		configFile = "config/basic.conf";

	if (argc >= 2)
		configFile = argv[1];
	
	Config	config(configFile);
	
	return (0);
}
