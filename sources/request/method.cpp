//#include "usefull.hpp"
#include "../../includes/usefull.hpp"

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

int HTTPRequest::HTTPMethod::parseMethod(const std::string req, const std::string *methods)
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

int HTTPRequest::HTTPMethod::parsePath(const std::string url)
{
	//std::cout << "3) url = " << url << std::endl;
	if (url[0] != '/')
		return 4;
	else
		this->_url = url;
	return 2;
}

int HTTPRequest::HTTPMethod::parseProtocol(const std::string protocol)
{
//	std::cout << "2) protocol = " << protocol << std::endl;
	if (strncmp(protocol.c_str(), "HTTP/1.0", 8) && strncmp(protocol.c_str(), "HTTP/1.1", 8))
		return 0;
	this->_httpv = protocol;
	return 1;
}

int HTTPRequest::method(char buf[30000])
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	HTTPResponse::STATUS code;
	HTTPRequest::HTTPMethod m;

	int i, j, k;

	getFct[0] = &HTTPRequest::get;
	getFct[1] = &HTTPRequest::post;
	getFct[2] = &HTTPRequest::mdelete;

	std::vector<std::string> request = splitThis(std::string(buf));
	for (i = 0; i < 3; i++)
	{
		if (request[i] == NULL)
		{
			//chek ou est lerreur. Si 1 arg voir method parse
			// si deux arg checker le fichier
			// si trois arg
		}
	}
	if ((i = m.parseMethod(request[0], methods)) == -1)
	{
		code.status(4, 4);
	}
	m._httpv = "HTTP/1.0";
	(this->*(getFct[i]))();
	if (!(k = m.parseProtocol(request[2])))
	{
		std::cout << RED << "[ treat the method then close() ]" << END << std::endl;
		code.status(4, 0);
	}
	j = m.parsePath(request[1]);
	if (j == 2)
		code.status(j, 0);
	//	std::cout << RED << "[ hola ]" << END << std::endl;
	return 1;
}