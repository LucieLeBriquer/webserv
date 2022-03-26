/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:25 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 14:54:25 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <fstream>
# include "Server.hpp"

typedef	std::vector<Server>	vecSrv;

class Config
{
	private:
		vecSrv	_servers;

		Config(const Config &config);
		Config(void);
		
	public:
		Config(std::string file);
		virtual ~Config();

		Config	&operator=(const Config &config);

		vecSrv	getServers(void) const;
};

std::ostream	&operator<<(std::ostream &o, const Config &config);

#endif
