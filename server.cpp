#include "inc/webserv.hpp"
#include <netdb.h> 
#define PORT 8080

int		main(void)
{
	int		server_fd, newSocket;
	struct sockaddr_in	address;
	socklen_t	   	    lenAddr;
	char                buf[1024] = {0};
	int                 byteCount;
	int yes = 1;
	HTTPRequest         treat;

	lenAddr = sizeof(address);

// Create a TCP socket ipv4
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Error with socket()" << std::endl;
		exit(EXIT_FAILURE);
	}
// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(yes)))
	{
		std::cout << "Error with setsockopt" << std::endl;
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.2");
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if (bind(server_fd, (struct sockaddr *)&address, (int)lenAddr) < 0)
	{
		std::cout << "Error with bind()" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		std::cout << "Error with listen()" << std::endl;
		exit(EXIT_FAILURE);
	}
	int i = 0;
	int j = 0;
	while (1)
	{
		std::cout << std::endl;
		std::cout << "++++++++++++ Waiting for new connection ++++++++++++" << std::endl;
		std::cout << std::endl;
		if ((newSocket = accept(server_fd, (struct sockaddr *)&address, &lenAddr)) < 0)
		{
			std::cout << "Error with accept()" << std::endl;
			exit(EXIT_FAILURE);
		}
		while (1)
		{
			byteCount = recv(newSocket, buf, sizeof(buf), 0);
			std::cout << buf << std::endl;
			i = treat.method(buf);
			byteCount = recv(newSocket, buf, sizeof(buf), 0);
			std::cout << buf << std::endl;
			j = treat.header(buf);
			if (i == -1)
			{
				//en fonction du user-agent : curl ou telnet ou mozilla  (curl/7.68.0)
				std::cout << "Connection closed by foreign host." << std::endl;
				exit(EXIT_FAILURE);
			}
			send(newSocket, "Hello world!", 12, 0);
			std::cout << "---------- Hello message sent ----------" << std::endl;
			// close(new_socket);
		}
	}
	return 0;
}
