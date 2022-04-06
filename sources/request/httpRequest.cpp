#include "usefull.hpp"

void HTTPRequest::get(void ){
	this->_method = "GET";
}

void HTTPRequest::post(void ){
	this->_method  = "POST";
}

void HTTPRequest::mdelete(void ){
	this->_method  = "DELETE";
}

int HTTPRequest::method(std::string buf, STATUS *code, HTTPResponse *deliver)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	
	int i;

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
	this->_httpv = "HTTP/1.0";
	this->_url = "/";
	if ((i = this->parseMethod(request[0], methods)) == -1)
	{
		deliver->statusCode(code->status(4, 5), this->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	else
		(this->*(getFct[i]))();
	if (!this->parsePath(request[1]))
	{
		deliver->statusCode(code->status(4, 4), this->getFirstLine());
		if (arg != 3)
			return -1;
		return 1;
	}
	if (!this->parseProtocol(request[2]))
	{
		deliver->statusCode(code->status(4, 0), this->getFirstLine());
		return -1;
	}
	std::cout << "what ["<< _url << "] and " << _httpv << std::endl;
	deliver->statusCode(code->status(2, 0), this->getFirstLine());
	if (arg != 3)
		return -1;
	return 1;
}