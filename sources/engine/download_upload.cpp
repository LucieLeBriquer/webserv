/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   download_upload.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpascrea <lpascrea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:31:16 by lpascrea          #+#    #+#             */
/*   Updated: 2022/05/05 16:29:54 by lpascrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"

bool	verifUpload(HTTPResponse &response, HTTPHeader &header)
{
	if (strcmp(response.getMethod().c_str(), "POST") == 0)
	{
		if (strcmp(response.getUrl().c_str(), "/download.html") == 0)
		{
			if (strncmp(header.getContentType().c_str(), "multipart/form-data;", 20) == 0)
				return true;
		}
	}
	return false;
}

std::string	getNewFileName(Socket &sock)
{
	int			find, i;
	std::string	fileName = "html/upload/";
	
	find = sock.getBody().find("filename=\"");
	find += 10;
	i = find;
	while (sock.getBody()[i] != '\"')
	{
		fileName += sock.getBody()[i];
		i++;
	}
	return fileName;
}

void	isUpload(HTTPResponse &response, Socket &sock, HTTPHeader &header, std::string string)
{
	std::stringstream	upload(string);
	long				size;
	int					find;
	
	if (verifUpload(response, header))
	{
		std::ofstream	outFile(getNewFileName(sock).c_str(), std::ofstream::binary);
		
		find = string.find("\r\n\r\n");
		string.erase(0, find + 4);
		find = string.find("\r\n\r\n");
		string.erase(0, find + 4);
		upload.seekg(0, std::ios::end);
		size = upload.tellg();
		outFile.write(string.c_str(), size);
		outFile.close();
		upload.str(std::string());
		upload.clear();
	}
}

void	isDownloading(HTTPHeader &header, HTTPResponse &response)
{
	std::string	hdr = response.getHeader();
	std::string filename;

	if (strncmp(response.getUrl().c_str(), "/download/", 10) == 0)
	{
		filename = &(response.getUrl().c_str())[10];
		if (strcmp(header.getSecFetchDest().c_str(), "document") == 0 && strcmp(header.getSecFetchMode().c_str(), "navigate") == 0)
		{
			hdr += "\r\nContent-Disposition: attachment; filename=\"" + filename + "\"";
			// hdr += "\r\nContent-Type: application/octet-stream";
			hdr += "\r\nContent-Type: application/download";
			hdr += "\r\nContent-Description: File Transfer";
			response.setHeader(hdr);
		}
	}
}