#include <sys/socket.h>
#include <iostream>
#include <cstring>

int		main(void)
{
	int		server_fd;
	struct sockaddr	address;
	socklen_t	   	len_addr;

    len_addr = sizeof(address);
	if (server_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
	{
		std::cout << "Error with socket()" << std::endl;
		return 0;
	}
    memset((void *)&address, 0, len_addr);

	return 0;
}
