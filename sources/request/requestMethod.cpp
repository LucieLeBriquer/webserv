#include "usefull.hpp"
//#include "../../includes/usefull.hpp"

std::string	HTTPRequest::HTTPMethod::getProtocol( void )
{
	return this->_httpv;
}

int HTTPRequest::HTTPMethod::parseMethod(const std::string req, const std::string *methods)
{
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

int HTTPRequest::HTTPMethod::parsePath(const std::string url)
{
	if (url == "")
		return 1;
	if (url[0] != '/')
		return 0;
	else
		this->_url = url;
	return 1;
}

int HTTPRequest::HTTPMethod::parseProtocol(const std::string protocol)
{
//	std::cout << "2) protocol = " << protocol << std::endl;
	if (protocol == "")
		return 1;
	if (strncmp(protocol.c_str(), "HTTP/1.0", 8) && strncmp(protocol.c_str(), "HTTP/1.1", 8))
		return 0;
	this->_httpv = protocol;
	return 1;
}

int HTTPRequest::method(std::string buf, HTTPResponse *deliver, HTTPRequest::HTTPMethod *m, HTTPResponse::STATUS *code)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	
	int i, j, k;

	getFct[0] = &HTTPRequest::get;
	getFct[1] = &HTTPRequest::post;
	getFct[2] = &HTTPRequest::mdelete;

	std::vector<std::string> request = splitThis(buf);
	std::vector<std::string>::iterator it;
	int arg = 0;
	for (it = request.begin(); it != request.end(); it++)
	{
		if (*it != "")
			arg++;
	}
	std::cout << "arg = "<< arg << std::endl;
	m->_httpv = "HTTP/1.0";
	m->_url = "/";
	if ((i = m->parseMethod(request[0], methods)) == -1)
	{
		deliver->response(code->status(4, 5), m->getProtocol(), code, m);
		if (arg != 3)
			return -1;
		return 1;
	}
	else
		(this->*(getFct[i]))();
	if (!(j = m->parsePath(request[1])))
	{
		deliver->response(code->status(4, 4), m->getProtocol(), code, m);
		if (arg != 3)
			return -1;
		return 1;
	}
	if (!(k = m->parseProtocol(request[2])))
	{
		deliver->response(code->status(4, 0), m->getProtocol(), code, m);
		return -1;
	}
	if (arg != 3)
		return -1;
	deliver->response(code->status(2, 0), m->getProtocol(), code, m);
	return 1;
}