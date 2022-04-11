/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/10 11:18:29 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP

# include "Socket.hpp"
# include "httpResponse.hpp"
# include "Status.hpp"
# include "httpRequest.hpp"
# include "httpHeader.hpp"

# define	BAD_METHODE	-1
# define	GET			1
# define	POST		2
# define 	MAX_EVENTS	10
# define 	BUFFER_SIZE	1024
# define 	B_SIZE		5

int		setsocknonblock(int sock);
int		initSockets(Socket *sock, const Config config);
int		initEpoll(Socket *sock, const Config config);
int		socketMatch(int fde, Socket *sock);
Socket	*initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i);
int		requestReponse(int epollfd, int fde, Socket *sock, int sockNbr);
int		endRequest(std::string string, Socket *sock);

void	GetCGIfile(std::string *file, int *tot_size, char **env);

#endif
