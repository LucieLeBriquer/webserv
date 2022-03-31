/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:29 by lle-briq          #+#    #+#             */
/*   Updated: 2022/03/28 16:38:14 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "Block.hpp"

class Location : public Block
{
	private:
		std::string	_path;
		std::string	_cgiPass;

		bool		_cgiPassSet;

		bool	_setPath(std::string str);
		void	_setCgiPass(vecStr words);
		void	_fillOneInfo(std::string str);

		int		_keywordNumber(std::string str);

	public:
		Location(void);
		Location(std::string str);
		Location(const Location &location);
		virtual ~Location();

		Location	&operator=(const Location &location);

		// getters
		std::string	getPath(void) const;
		std::string	getCgiPass(void) const;

		// static
		static const int	nbKeywords = 8;
		static std::string	keywords[nbKeywords];
};

std::ostream	&operator<<(std::ostream &o, const Location &location);

typedef void (Location::*setFunc3)(vecStr words);

#endif
