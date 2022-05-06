/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/06 11:38:57 by masboula         ###   ########.fr       */
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
# define	HEAD		3
# define	DELETE		4
# define	OPTIONS		5
# define 	MAX_EVENTS	10
# define 	BUFFER_SIZE	1024
# define 	B_SIZE		5

int			setsocknonblock(int sock);
int			initSockets(Socket *sock, const Config config);
int			initEpoll(Socket *sock, const Config config);
int			socketMatch(int fde, Socket *sock);
Socket		*initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i);
int			requestReponse(int epollfd, int fde, Socket *sock);
int			endRequest(std::string string, Socket &sock);
std::string	getHead(std::string buf);

std::string	headerForCgi(std::string header, Socket &sock, int sockNbr);
void        setEnvForCgi(Socket &sock, HTTPResponse &response, int sockNbr, HTTPHeader &header);
int			GetCGIfile(Socket &sock, std::string cgi);
void    	ft_free_env_arg(char ***env, Socket *sock);
int     	mallocEnv(char ***env, Socket &sock);

int			sendDefaultPage(int fde, HTTPResponse &response);
int			sendAutoindexPage(int fde, HTTPResponse &response, std::string path, std::string root);

static int chunk_size = 0;

#endif
