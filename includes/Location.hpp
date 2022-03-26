/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:29 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/26 14:54:34 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "usefull.hpp"

class Location
{
	private:
		std::string	_root;
		std::string	_index;
		size_t		_maxClientBody;
		vecInt		_methods;
		std::string	_redirUrl;
		bool		_autoindex;
		bool		_formatOk;

	public:
		Location(void);
		Location(std::string str);
		Location(const Location &location);
		virtual ~Location();

		Location	&operator=(const Location &location);

		bool	wellFormatted(void) const;
};

std::ostream	&operator<<(std::ostream &o, const Location &location);

#endif
