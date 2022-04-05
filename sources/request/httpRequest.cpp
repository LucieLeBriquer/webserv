#include "usefull.hpp"

//#include "../../includes/usefull.hpp"

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

int HTTPRequest::method(std::string buf, HTTPMethod *m, STATUS *code)
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
	this->_httpv = "HTTP/1.0";
	this->_url = "/";
	if ((i = m->parseMethod(request[0], methods)) == -1)
	{
		code->statusCode(code->status(4, 5), m->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	else
		(this->*(getFct[i]))();
	if (!(j = m->parsePath(request[1])))
	{
		code->statusCode(code->status(4, 4), m->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	if (!(k = m->parseProtocol(request[2])))
	{
		code->statusCode(code->status(4, 0), m->getFirstLine());
		return -1;
	}
	if (arg != 3)
		return -1;
	code->statusCode(code->status(2, 0), m->getFirstLine());
	return 1;
}