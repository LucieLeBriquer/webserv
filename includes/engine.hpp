/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/04/25 18:07:54 by user42           ###   ########.fr       */
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
# define 	MAX_EVENTS	10
# define 	BUFFER_SIZE	1024
# define 	B_SIZE		5

# define    CGI_PASS    "sock.getCgiPass(sockNbr, response.getUrl())"
# define    FILE_ASKED  "sock.getRealUrl(sockNbr, response.getUrl())"


int			setsocknonblock(int sock);
int			initSockets(Socket *sock, const Config config);
int			initEpoll(Socket *sock, const Config config);
int			socketMatch(int fde, Socket *sock);
Socket		*initConnection(Socket *sock, struct epoll_event ev, int epollfd, int i);
int			requestReponse(int epollfd, int fde, Socket *sock);
int			endRequest(std::string string, Socket &sock);
std::string	getHead(std::string buf);

void        setEnvForCgi(Socket &sock, HTTPResponse &response, int sockNbr);
int			GetCGIfile(Socket &sock, int fde, std::string cgi, std::string file);
void    	ft_free_env_arg(char ***env, char ***arg, Socket *sock);
int     	mallocEnv(char ***env, Socket &sock);
int     	mallocArg(char ***arg, std::string cgi, std::string file);

int			sendDefaultPage(int fde, HTTPResponse &response);
int			sendAutoindexPage(int fde, HTTPResponse &response, std::string path, std::string root);

#endif
