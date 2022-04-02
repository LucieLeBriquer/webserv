//#include "usefull.hpp"
#include "../../includes/usefull.hpp"

void HTTPResponse::STATUS::err4xx(int type)
{
    this->_code = 400 + type;
    std::cout << "Error " << this->_code << std::endl;
}

void HTTPResponse::STATUS::status2xx(int type)
{
    this->_code = 200 + type;
    std::cout << "Error " << this->_code << std::endl;
}

void HTTPResponse::STATUS::err5xx(int type)
{
    this->_code = 500 + type;
    std::cout << "Error " << this->_code << std::endl;
}

int HTTPResponse::STATUS::status(int code, int type)
{
    std::map<int, int> status;

    status[4] = 0;
    status[2] = 1;
    status[5] = 2;
    getStatus[0] = &HTTPResponse::STATUS::err4xx;
    getStatus[1] = &HTTPResponse::STATUS::status2xx;
    getStatus[2] = &HTTPResponse::STATUS::err5xx;

    int x = status[code];
    (this->*(getStatus[x]))(type);
    return this->_code;
}