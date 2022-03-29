#include "webserv.hpp"

void HTTPRequest::get(void )
{
    printf("HERE\n");
    this->_method = "GET";
    // ofstream header()
}

void HTTPRequest::post(void )
{
    printf("THERE\n");
    this->_method  = "POST";

}
void HTTPRequest::mdelete(void )
{
    printf("HOHEE\n");
    this->_method  = "DELETE";

}

int HTTPRequest::parseMethod(const std::string cmd, const std::string *methods)
{
    int i;

    i = 0;

    while (cmd[i])
    {
        if (!isupper(cmd[i]))
        {
        printf("i = %d, what [%c]\n", i, cmd[i]);
            return (-1);
        }
        i++;
    }
    //i -= 2;
    printf("i = %d, meth [%s]\n", i, cmd.c_str());
    for (int j = 0; j < 4; j++)
    {
        if (!strncmp(cmd.c_str(), methods[j].c_str(), i))
        {
            return (j);
        }
    }
    printf("but what\n");
    return (-1);
}

int HTTPRequest::parsePathNProtocol(const std::string str, int err)
{
    ERR4 problem;
    int i;
    if (err == -1)
        return (-1);

    i = this->_method.length();
    std::string cmd = copystr(str, i);
    // if (cmd[i] == '\n' || cmd[i] == '\r')
    // {
    //     std::cout << RED << "[ treat the method then close() ]" << END << std::endl;
    //     return 1;
    // }
    std::cout << "avant " << this->_path << std::endl;
    if (cmd[i - 1] == ' ' && cmd.find('/'))
    {
        std::cout << "alors " << this->_path << std::endl;

        if ((cmd[i + 1] >= 'a' && cmd[i + 1] <= 'z') || 
            (cmd[i + 1] >= 'A' && cmd[i + 1] <= 'Z'))
        {
            this->_path = splitThis(cmd, i);
        std::cout << "path split = " << this->_path << std::endl;
        }
        else if (cmd[i + 1] == ' ')
        {
            this->_path = '/';
        std::cout << "path / = " << this->_path << std::endl;
        }
    }
    else
    {
        problem.error4xx(copystr(cmd, i), 4);
        return -1;
    }
    std::cout << "ici " << this->_path << std::endl;
    std::string cmd1 = copystr(cmd, this->_path.length() + 1);
    if (cmd1.find('\n'))
    {
        std::cout << RED << "[ insert default header then close() ]" << END << std::endl;
        this->_protocolv = "HTTP/1.0";
    }
    else
    {
        std::cout << RED << "[ recup l'entree et la traite apres ]" << END << std::endl;
        this->_protocolv = cmd1;
    }
    return 1;
}

int HTTPRequest::request(char buf[1024])
{
    std::string methods[3] = {"GET", "POST", "DELETE"};

    std::string methds;    
    std::string url;    
    std::string protocol;
    
    int i, j;

    getFct[0] = &HTTPRequest::get;
    getFct[1] = &HTTPRequest::post;
    getFct[2] = &HTTPRequest::mdelete;

    j = -1;
    std::string cmd[3] = splitThis(std::string(buf));
    if ((i = parseMethod(cmd[0], methods)) != -1)
        (this->*(getFct[i]))();
    if (cmd[1] && cmd[2])
        j = parsePathNProtocol(cmd[1], cmd[2] i);
    if (i == -1 || j == -1)
        return (-1);
    return (0);
}