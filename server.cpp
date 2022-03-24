#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <cstring>

#define PORT 8080

int		main(void)
{
	int		server_fd, new_socket;
	struct sockaddr_in	address;
	socklen_t	   	    len_addr;
    char                buf[1024] = {0};
    int                 byte_count;
    int                 yes = 1;

    len_addr = sizeof(address);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Error with socket()" << std::endl;
		exit(EXIT_FAILURE);
	}

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)) < 0)
    {
        std::cout << "Error with setsockopt()" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd, (struct sockaddr *)&address, (int)len_addr) < 0)
    {
        std::cout << "Error with bind()" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        std::cout << "Error with listen()" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        std::cout << std::endl;
        std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
        std::cout << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &len_addr)) < 0)
        {
            std::cout << "Error with accept()" << std::endl;
            exit(EXIT_FAILURE);
        }
        byte_count = recv(new_socket, buf, sizeof(buf), 0);
        std::cout << buf << std::endl;
        send(new_socket, "Hello world!", 12, 0);
        std::cout << "---------- Hello message sent ----------" << std::endl;
        // close(new_socket);
    }
    
	return 0;
}
