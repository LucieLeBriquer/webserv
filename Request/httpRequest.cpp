#include "../inc/webserv.hpp"

int HTTPRequest::request(char buf[1024])
{
    treat.method(buf);
    treat.header(buf);
}
