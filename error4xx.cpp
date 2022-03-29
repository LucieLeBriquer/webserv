#include "errors.hpp"

void ERR4::error4xx(std::string str, int code)
{
    std::cout << str << " 40" << code << " Not Found" << std::endl; 
}