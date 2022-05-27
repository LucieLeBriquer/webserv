/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-briq <lle-briq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 13:50:27 by lle-briq          #+#    #+#             */
/*   Updated: 2022/05/27 14:04:00 by lle-briq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "engine.hpp"

static int			getErrorNb(std::string code)
{
	for (int i = 0; i < nbError; i++)
	{
		if (code == errorNb[i])
			return (i);
	}
	return (ERR);
}

static std::string	getErrorMsg(std::string code)
{
	int	nb = getErrorNb(code);

	if (nb == ERR)
		return ("");
	return (errorMsg[nb]);
}

static std::string	getErrorExplanation(std::string code)
{
	int	nb = getErrorNb(code);

	if (nb == ERR)
		return ("");
	return (errorExplanation[nb]);
}

static std::string	getPageContent(std::string code)
{
	std::string	content;
	
	content = "<!DOCTYPE HTML>\n\n\t<html>\n\t<head>";
	content += "\n\t<title>Webserv</title>";
	content += "\n\t<link rel=\"shortcut icon\" href=\"https://lucie-lebriquer.fr/webserv/favicon.png\"/>";
	content += "\n\t<meta charset=\"utf-8\"/>";
	content += "\n\t<link rel=\"stylesheet\" href=\"https://lucie-lebriquer.fr/webserv/default.css\"/>";
	content += "\n\t<script src=\"https://kit.fontawesome.com/58cd01063a.js\" crossorigin=\"anonymous\"></script>";
	content += "\n\t</head>\n<header>";
	content += "\n\t<div class=\"logo\">";
	content += "\n\t\t<img src=\"https://lucie-lebriquer.fr/webserv/logo.png\">";
	content += "\n\t\t<div class=\"titlename\">Webserv</div>";
	content += "\n\t\t<div class=\"title\">euses - default</div>";
	content += "\n\t</div>\n\t<span class=\"buttonTab\">";
	content += "\n\t\t<a href=\"https://github.com/LucieLeBriquer/webserv.git\"><i class=\"fab fa-github\"></i><span class=\"tabname\">Source</span></a>";
	content += "\n\t</span>\n</header>\n\n<body>\n<div class=\"vertical\">";
	content += "\n\t<center>\n\t<h6 class=\"title404_1\">";
	content += code;
	content += "</h6><br><br><br>\n\t<h6 class=\"title404_2\">";
	content += getErrorMsg(code);
	content += "</h6><br><br><br>\n\t";
	content += getErrorExplanation(code);
	content += "\n</div>\n</body>\n\n<footer>\n\t<ul class=\"footer\">\n\t\t<li></li>";
	content += "\n\t\t<li><a href=\"https://github.com/MassiliaB\" target=\"_blank\"><i class=\"fab fa-github\"></i> masboula</a></li>";
	content += "\n\t\t<li><a href=\"https://github.com/loupascreau\" target=\"_blank\"><i class=\"fab fa-github\"></i> lpascrea</a></li>";
	content += "\n\t\t<li><a href=\"https://github.com/LucieLeBriquer\" target=\"_blank\"><i class=\"fab fa-github\"></i> lle-briq</a></li>";
	content += "\n\t\t<li></li>\n\t</ul>\n</footer>\n</html>\n";
	
	return (content);
}

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

	body = getPageContent(toString(response.getStatusNb()));
	
	header = response.getProtocol() + ' ' + response.getStatus() + "\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + toString(body.size()) + "\r\n";
	header += "Date: " + timeStr;
	
	toSend = header + "\r\n\r\n" + body;
	
	if (LOG_LEVEL >= LVL_INFO)
		std::cout << ORANGE << "[Sending] " << END << "default error page to " << fde << std::endl;
	if (LOG_LEVEL >= LVL_RESPONSE)
	{
		std::cout << GRAY << std::endl << "<----------- Response ----------->" << std::endl;
		std::cout << header << END << std::endl << std::endl;
	}
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
	else if (file != "../")
		res = file + "</a>" + std::string(len - file.size(), ' ');
	else
		res = file + "</a>";
	return (res);
}

static std::string	getTimeAndSize(const std::string file, int type)
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
	return (std::string(44, ' ') + "-\n");
}

static std::string	getFileInfoFormat(const std::pair<std::string,int> file, int type, const std::string &directory)
{
	std::string res;

	if (type == DIRECTORY)
		res = "<a class=\"directory\" href=\"" + file.first + "/\">" + printFileName(file.first + "/");
	else
		res = "<a class=\"file\" href=\"" + file.first + "\">" + printFileName(file.first);
	if (file.first != "..")
		res += " " + getTimeAndSize(directory + file.first, type);
	else
		res += "\n";
	return (res);
}

static std::string	getDirectories(const vecFiles &files, const std::string &directory)
{
	std::string	dirPart = "";

	for (size_t i = 0; i < files.size(); i++)
	{
		if (files[i].second == DIRECTORY)
			dirPart += getFileInfoFormat(files[i], DIRECTORY, directory);
	}
	return (dirPart);
}

static std::string	getRegularFiles(const vecFiles &files, const std::string &directory)
{
	std::string	filePart = "";

	for (size_t i = 0; i < files.size(); i++)
	{
		if (files[i].second == REGFILE)
			filePart += getFileInfoFormat(files[i], REGFILE, directory);
	}
	return (filePart);
}

int	sendAutoindexPage(int fde, HTTPResponse &response, std::string path, std::string root)
{
	std::string		body;
	std::string		header;
	std::string		toSend;
	vecFiles		files;
	std::string		timeStr;
	time_t 			rawtime;
	std::string		directory;
	
	time(&rawtime);
	timeStr = ctime(&rawtime);
	timeStr = timeStr.substr(0, timeStr.size() - 1);
	
	if (root.empty())
		directory = "./" + removeSlash(path);
	else
		directory = "./" + root + "/" + removeSlash(path);
	
	getDirectoryContent(directory, files);

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
	body += getDirectories(files, directory);
	body += getRegularFiles(files, directory);
	body += "</pre>\n<hr>\n</body>\n</html>";
	
	header = response.getProtocol() + ' ' + response.getStatus() + "\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: " + toString(body.size()) + "\r\n";
	header += "Date: " + timeStr;
	
	toSend = header + "\r\n\r\n" + body;

	if (LOG_LEVEL >= LVL_INFO)
		std::cout << ORANGE << "[Sending] " << END << "autoindex page to " << fde << std::endl;
	if (LOG_LEVEL >= LVL_RESPONSE)
	{
		std::cout << GRAY << std::endl << "<----------- Response ----------->" << std::endl;
		std::cout << header << END << std::endl << std::endl;
	}
	if (send(fde, toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (ERR);
	}
	return (OK);
}
