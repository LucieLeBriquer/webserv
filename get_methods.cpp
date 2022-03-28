#include "methods.hpp"

void HTTPRequest::get(void )
{
    printf("HERE\n");
    _method = "GET";
    // ofstream header()
}

void HTTPRequest::post(void )
{
    printf("THERE\n");
    _method  = "POST";

}
void HTTPRequest::mdelete(void )
{
    printf("HOHEE\n");
    _method  = "DELETE";

}

int parseMethod(const std::string cmd, const std::string *methods)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (!isupper(cmd[i]) && !cmd.find('\n'))
            return (-1);
        i++;
    }
    i -= 2;
    printf("i = %d, meth [%s]\n", i, methods[0].c_str());
    for (int j = 0; j < 4; j++)
    {
        if (!strncmp(cmd.c_str(), methods[j].c_str(), i))
            return (j);
    }
    return (-1);
}

int HTTPRequest::parsePathNProtocol(const std::string cmd, int err)
{
    int i;
    if (err == -1)
        return (-1);

    i = 0;
    while (isupper(cmd[i]))
        i++;
    while (cmd[i])
    {
        if (cmd[i - 1] == ' ' && cmd.find('/'))
            strcpy((cmd + i).c_str()
        i++;
    }

}

int HTTPRequest::request(char buf[1024])
{
    std::string methods[3] = {"GET", "POST", "DELETE"};
    std::string cmd = std::string(buf);
    int i, j;

    getFct[0] = &HTTPRequest::get;
    getFct[1] = &HTTPRequest::post;
    getFct[2] = &HTTPRequest::mdelete;

    if ((i = parseMethod(cmd, methods)) != -1)
        (this->*(getFct[i]))();
    j = parsePathNProtocol(cmd, i);
    if (i == -1 || j == -1)
        return (-1);
    return (0);
}