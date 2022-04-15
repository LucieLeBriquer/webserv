/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 13:50:27 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/15 14:26:31 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "engine.hpp"

int	sendDefaultPage(int fde, HTTPResponse &response)
{
	std::string		body;
	std::string		header;
	std::string		toSend;
	std::string		timeStr;
	time_t 			rawtime;
	
	time(&rawtime);
	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	
	body = "<!DOCTYPE HTML>\n\n<html>\n<body>\n";
	body += "<style>\n\t* {\n\t\tfont-family: Helvetica, sans-serif;\n";
	body += "\t\tfont-size: 2em;\n";
	body += "}\n</style>\n";
	body += "\t<h2 align =\"center\">Error ";
	body += toString(response.getStatusNb()) + "<h2>\n</body>\n</html>";
	
	header = "HTTP/1.1 " + response.getStatus() + "\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + toString(body.size()) + "\r\n";
	header += "Date: " + timeStr;
	
	toSend = header + "\r\n\r\n" + body;

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde << std::endl;
	std::cout << toSend << std::endl;
	if (send(fde, toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}
