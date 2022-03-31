//#include "usefull.hpp"
#include "../../includes/usefull.hpp"

void HTTPResponse::STATUS::err4xx(int type)
{
    HTTPResponse response;

    response._statusCode = 400 + type;
    std::cout << "400 ===" << response._statusCode << std::endl;
}

void HTTPResponse::STATUS::status2xx(int type)
{
    this->_code = 200;
    std::cout << "200 OK " << this->_code << std::endl; 
}

void HTTPResponse::STATUS::err5xx(int type)
{
    this->_code = 500;
    std::cout << " 50" << this->_code << " Not Found" << std::endl; 
}

void HTTPResponse::STATUS::status(int code, int type)
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
    (this->*(getStatus[x]))(type);
    response.header();
}