#include "methods.hpp"

void Requests::treatRequest(char buf[1024])
{
    std::string methods[3] = {"GET", "POST", "DELETE"};
    int i;

    getFct[0] = Requests::get;
    getFct[1] = Requests::post;
    getFct[2] = Requests::mdelete;

    for (i = 0; i < 4; i++)
    {
        if (parseContent(buf) == )
    }
}