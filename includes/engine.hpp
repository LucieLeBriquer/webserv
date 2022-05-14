/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:25:03 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/14 14:38:38 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP
# define BUFFER_SIZE 1024
# define B_SIZE 5
# define END_REQUEST 1
# define CLOSE_CONNECTION 2
# define BAD_REQUEST 3
# include "Socket.hpp"

// epoll
int			initEpoll(Socket &sock);
int			initConnection(Socket &sock, int i);

// message
int			requestReponse(int fde, Socket &sock);

// cgi
std::string	headerForCgi(std::string header, Client &client, const std::string servName);
void        setEnvForCgi(Socket &sock, int sockNbr, Client &client);
int			getCGIfile(std::string cgi, Client &client);
int			getRightFile(Socket &sock, int sockNbr, Client &client);

// env
void    	freeEnv(char ***env, Client &client);
int     	mallocEnv(char ***env, Client &client);

// send
int			sendResponse(Client &client, Socket &sock, int sockNbr);
int			sendDefaultPage(int fde, HTTPResponse &response);
int			sendAutoindexPage(int fde, HTTPResponse &response, std::string path, std::string root);

// utils
void		isDownloading(HTTPHeader &header, HTTPResponse &response);
int			endRequest(Client &client);

#endif
