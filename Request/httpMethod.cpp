#include "../inc/webserv.hpp"

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

int HTTPRequest::parseMethod(const std::string req, const std::string *methods)
{
//	std::cout << "2) meth = " << req << std::endl;
	int i;
	for (i = 0; req[i]; i++)
	{
		if (!isupper(req[i]))
			return (-1);
	}
	for (int j = 0; j < 4; j++)
	{
		if (!strncmp(req.c_str(), methods[j].c_str(), i))
			return (j);
	}
	return (-1);
}

int HTTPRequest::parsePath(const std::string url)
{
	//std::cout << "3) url = " << url << std::endl;
	if (url[0] != '/')
		return 4;
	else
		this->_path = url;
	return 2;
}

int HTTPRequest::parseProtocol(const std::string protocol)
{
//	std::cout << "2) protocol = " << protocol << std::endl;
	if (strncmp(protocol.c_str(), "HTTP/1.0", 8) && strncmp(protocol.c_str(), "HTTP/1.1", 8))
		return 4;
	else
		this->_httpv = protocol;
	return 0;
}

int HTTPRequest::method(char buf[1024])
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	HTTPResponse::STATUS code;

	int i, j, k;

	getFct[0] = &HTTPRequest::get;
	getFct[1] = &HTTPRequest::post;
	getFct[2] = &HTTPRequest::mdelete;

	std::vector<std::string> request = splitThis(std::string(buf));
	if ((i = parseMethod(request[0], methods)) == -1)
	{
		std::cout << RED << "[ treat the method then close() ]" << END << std::endl;
		code.status(4);
		return -1;
	}
	this->_httpv = "HTTP/1.0";
	(this->*(getFct[i]))();
	if ((k = parseProtocol(request[2])) > 0)
	{
		std::cout << RED << "[ treat the method then close() ]" << END << std::endl;
		code.status(k);
		return -1;
	}
	j = parsePath(request[1]);
	code.status(j);
	return 1;
}