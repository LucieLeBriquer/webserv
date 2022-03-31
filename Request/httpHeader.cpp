#include "../inc/webserv.hpp"

int HTTPRequest::header(char buf[1024])
{
	std::string header[3] = {"host", "user-agent", "accept"};
	(void)buf;
	return (1);
}