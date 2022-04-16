/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 13:50:27 by lle-briq          #+#    #+#             */
/*   Updated: 2022/04/16 15:53:47 by lle-briq         ###   ########.fr       */
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
	body += toString(response.getStatusNb()) + "</h2>\n</body>\n</html>";
	
	header = "HTTP/1.1 " + response.getStatus() + "\r\n"; // a remplacer par getProtocol
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + toString(body.size()) + "\r\n";
	header += "Date: " + timeStr;
	
	toSend = header + "\r\n\r\n" + body;
	
	std::cout << ORANGE << "[Sending] " << END << "data to " << fde << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << header << std::endl;
	std::cout << "====================================================" << std::endl;
	if (send(fde, toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}

static void	getDirectoryContent(std::string path, vecFiles &files)
{
	directory	*rep = NULL;
    fileInfo	*file = NULL;

    rep = opendir(path.c_str());
	if (rep == NULL)
		return ;
	
	while ((file = readdir(rep)) != NULL)
	{
		if ((file->d_name[0] != '.' || strcmp(file->d_name, "..") == 0)
			&& ((int)file->d_type == 4 || (int)file->d_type == 8))
		{
			if ((int)file->d_type == 4)
				files.push_back(std::pair<std::string, int>(file->d_name, DIRECTORY));
			else
				files.push_back(std::pair<std::string, int>(file->d_name, REGFILE));
		}
	}
    if (closedir(rep) == -1)
		return ;

	std::sort(files.begin(), files.end());
}

static std::string	printFileName(std::string file, size_t len = 52)
{
	std::string	res = "";

	if (file.size() > 52)
		res = file.substr(0, len - 3) + "..></a>";
	else
		res = file + "</a>" + std::string(len - file.size(), ' ');
	return (res);
}

static std::string	getTimeAndSize(std::string file, int type)
{
	std::string	time;
	std::string	sizeFile = "-";
	struct stat sb;
	std::string res = "";

    int rc = stat(file.c_str(), &sb);
	if (rc == 0)
	{
		time = ctime(&sb.st_mtime);
		time = time.substr(0, time.size() - 1);
		res = time;
		if (type == REGFILE)
			sizeFile = toString(sb.st_size);
		if (sizeFile.size() > 21)
			res += sizeFile.substr(0, 21);
		else
			res += std::string(21 - sizeFile.size(), ' ') + sizeFile;
		return (res + "\n");
	}
	return (std::string(45, ' ') + "-\n");
}

static std::string	getFileInfoFormat(const std::pair<std::string,int> file, int type)
{
	std::string res;

	if (type == DIRECTORY)
		res = "<a class=\"directory\" href=\"" + file.first + "/\">" + printFileName(file.first + "/");
	else
		res = "<a class=\"file\" href=\"" + file.first + "\">" + printFileName(file.first);
	res += " " + getTimeAndSize(file.first, type);
	return (res);
}

static std::string	getDirectories(const vecFiles &files)
{
	std::string	dirPart = "";

	for (size_t i = 0; i < files.size(); i++)
	{
		if (files[i].second == DIRECTORY)
			dirPart += getFileInfoFormat(files[i], DIRECTORY);
	}
	return (dirPart);
}

static std::string	getRegularFiles(const vecFiles &files)
{
	std::string	filePart = "";

	for (size_t i = 0; i < files.size(); i++)
	{
		if (files[i].second == REGFILE)
			filePart += getFileInfoFormat(files[i], REGFILE);
	}
	return (filePart);
}

int	sendAutoindexPage(int fde, HTTPResponse &response, std::string path)
{
	std::string		body;
	std::string		header;
	std::string		toSend;
	vecFiles		files;
	std::string		timeStr;
	time_t 			rawtime;
	
	time(&rawtime);
	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);

	getDirectoryContent("." + path, files);
	
	body = "<!DOCTYPE HTML>\n\n<html>\n<body>\n";
	body += "<style>\n\t* {\n\t\tline-height: 1.4em;\n";
	body += "\t\tbackground-color: #221f22;\n";
	body += "\t\tcolor: #7e7c7e;\n";
	body += "\t\tborder-color: #7e7c7e;\n";
	body += "\t\tfont-size: 1.03em;\n";
	body += "\t}\n";
	body += "\t.directory, .path {\n\t\tcolor: #d8b241;\n\t}\n";
	body += "\th1 {\n\t\tfont-family: Helvetica, sans-serif;\n";
	body += "\t\tfont-size: 2em;\n";
	body += "\t}\n";
	body += "\t.path {\n\t\tdisplay: inline;\n\t}\n";
	
	body += "</style>\n";
	body += "<h1>Index of <div class=\"path\">" + path + "</div></h1>\n<hr>\n<pre>\n";
	body += getDirectories(files);
	body += getRegularFiles(files);
	body += "</pre>\n<hr>\n</body>\n</html>";
	
	header = "HTTP/1.1 " + response.getStatus() + "\r\n"; // a remplacer par getProtocol
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + toString(body.size()) + "\r\n";
	header += "Date: " + timeStr;
	
	toSend = header + "\r\n\r\n" + body;

	std::cout << ORANGE << "[Sending] " << END << "data to " << fde << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << header << std::endl;
	std::cout << "====================================================" << std::endl;
	if (send(fde, toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}
