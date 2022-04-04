#include "usefull.hpp"
//#include "../../includes/usefull.hpp"

int HTTPRequest::header(char buf[30000])
{
	HTTPRequest::HTTPHeader h;
	std::string header[3] = {"host:", "user-agent:", "accept:"};
	std::string *info[3] = {&h._host, &h._useragent, &h._accept};
	std::string head(buf);
	int i, j;

	for (i = 0; i < 3; i++)
	{
		if (!strncasecmp(buf, header[i].c_str(), header[i].length()))
			break;
	}
	if (i == 3)
		return (-1);

	j = header[i].length();
	if (buf[j] == ' ')
		j++;
	int pos = j;
	while (buf[j] != '\n' && buf[j] != '\r' && buf[j] != ' ')
	{
	    j++;
	}
	int len = j - pos;
	char tmp[len + 1];
	head.copy(tmp, len, pos);
	tmp[len] = '\0';
//	std::cout << "tmp =" << tmp << std::endl;
	std::string value(tmp);
	*info[i] = value;
	if (i == 0)
	{
		if (value != "127.0.0.2")
			return (-1);
	}
	return (1);
}