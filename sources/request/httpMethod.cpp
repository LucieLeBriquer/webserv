#include "usefull.hpp"

int HTTPRequest::parseMethod(const std::string req, const std::string *methods)
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

int HTTPRequest::parsePath(const std::string url)
{
	if (url == "")
		return 1;
	if (url[0] != '/')
		return 0;
	else
		this->_url = url;
	return 1;
}

int HTTPRequest::parseProtocol(const std::string protocol)
{
	if (protocol == "")
		return 1;
	if (strncmp(protocol.c_str(), "HTTP/1.0", 8) && strncmp(protocol.c_str(), "HTTP/1.1", 8))
		return 0;
	this->_httpv = protocol;
	return 1;
}
