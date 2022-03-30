# include "inc/webserv.hpp"

std::string copystr(std::string str, int start)
{
    std::string dest;

    for (int j = 0; str[start]; start++, j++)
        dest[j] = str[start];
    return dest;
}


std::vector<std::string> splitThis(std::string str)
{
    std::string command(str);
    std::vector<std::string> splited(3);

    int i = 0;
    std::stringstream ssin(command);
    while (ssin.good() && i < 3)
    {
        ssin >> splited[i];
        i++;
    }
 
    std::cout << "uno [" << splited[0] << "]" << std::endl;
    std::cout << "dos ["<< splited[1] << "]" << std::endl;
    std::cout << "tres ["<< splited[2] << "]" << std::endl;

    return splited;
}
