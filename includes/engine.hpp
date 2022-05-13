/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/13 14:29:49 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP
# include "Socket.hpp"
 
# define	BAD_METHODE	-1
# define	GET			0
# define	POST		1
# define	DELETE		2
# define	HEAD		3
# define	OPTIONS		4
# define 	BUFFER_SIZE	1024
# define 	B_SIZE		5

# define	END_REQUEST 1
# define	CLOSE_CONNECTION 2
# define	BAD_REQUEST 3

int			initEpoll(Socket &sock);
int			initConnection(Socket &sock, int i);
int			requestReponse(int fde, Socket &sock);

int			endRequest(FILE *file, Socket &sock, std::string string);

std::string	getHead(std::string buf);
size_t		getFdSize(int fd);

std::string	headerForCgi(std::string header, Socket &sock, int sockNbr);
void        setEnvForCgi(Socket &sock, int sockNbr, Client &client);
int			getCGIfile(Socket &sock, std::string cgi, Client &client);
void    	freeEnv(char ***env, Client &client);
int     	mallocEnv(char ***env, Client &client);

int			sendDefaultPage(int fde, HTTPResponse &response);
int			sendAutoindexPage(int fde, HTTPResponse &response, std::string path, std::string root);

void		isDownloading(HTTPHeader &header, HTTPResponse &response);

#endif
