/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 14:54:29 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/26 14:27:38 by lle-briq         ###   ########.fr       */
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
		const std::string	getPath(void) const;
		const std::string	getCgiPass(void) const;
		
		bool				isCgiPassSet(void) const;

		// static
		static const int			nbKeywords = 8;
		static const std::string	keywords[nbKeywords];
};

std::ostream	&operator<<(std::ostream &o, const Location &location);

typedef void (Location::*locSet)(vecStr words);

#endif
