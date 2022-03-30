#include "../inc/webserv.hpp"

void HTTPResponse::STATUS::err4xx(int code)
{
    this->_code = 404;
    std::cout << " 404 " << code << " Not Found" << std::endl; 
}

void HTTPResponse::STATUS::status2xx(int code)
{
    this->_code = 200;
    std::cout << "200 OK " << code << std::endl; 
}

void HTTPResponse::STATUS::err5xx(int code)
{
    this->_code = 500;
    std::cout << " 50" << code << " Not Found" << std::endl; 
}

void HTTPResponse::STATUS::status(int code)
{
    std::map<int, int> status;
    HTTPResponse response;

    status[4] = 0;
    status[2] = 1;
    status[5] = 2;
    getStatus[0] = &HTTPResponse::STATUS::err4xx;
    getStatus[1] = &HTTPResponse::STATUS::status2xx;
    getStatus[2] = &HTTPResponse::STATUS::err5xx;

    int x = status[code];
    response.header();
    (this->*(getStatus[x]))(code);
}