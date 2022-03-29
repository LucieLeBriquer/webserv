# include "webserv.hpp"

std::string copystr(std::string str, int start)
{
    std::string dest;

    for (int j = 0; str[start]; start++, j++)
        dest[j] = str[start];
    delete str;
    return dest;
}

std::string splitThis(const std::string str)
{
    std::string splited[3];

   // std::cout << str << std::endl;
    for (int j = 0; str[i]; i++, j++)
    {
        if (str[i] != '/' || (str[i] < 'a' || str[i] > 'z') || 
            (str[i] < 'A' || str[i] > 'Z'))
            break ;
        splited[j] = str[i];
    }
  //  std::cout << splited << std::endl;
    return splited;
}

std::string *splitThis(const std::string str)
{
    std::string splited[3];

    int i = 0;
    int j = 0;
    while (str[i])
    {
        splited[0][j] = str[i];
        if (str[i] == ' ' || str[i] == '\r')
            break;
        i++;
        j++;
    }
    while (str[i] == ' ')
        i++;
    if (str[i] == '\r')
    {
        splited[1] = NULL;
        splited[2] = NULL;
        return splited;
    }
    std::cout << "splited de 0 = " << splited[0]<< std::endl;

    j = 0;
    while (str[i])
    {
        splited[1][j] = str[i];
        if (str[i] == ' ' || str[i] == '\r')
            break;
        i++;
        j++;
    }
    while (str[i] == ' ')
        i++;
    if (str[i] == '\r')
    {
        splited[2] = NULL;
        return splited;
    }
    std::cout << "splited de 1= " << splited[1]<< std::endl;

    j = 0;
    while (str[i])
    {
        splited[2][j] = str[i];
        if (str[i] == ' ' || str[i] == '\r')
            break;
        i++;
        j++;
    }
    std::cout << "splited de 2= " << splited[2]<< std::endl;

  //  std::cout << splited << std::endl;

    return splited;
}
