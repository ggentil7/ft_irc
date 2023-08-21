
#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>

int main()
{
	int socketFd, socketClient;
	struct sockaddr_in	ServerAddr;

	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(6669);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(ServerAddr.sin_zero), 8);

	bind(socketFd, (const struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
	std::cout << "BIND : " << socketFd << std::endl;

	listen(socketFd, 5);
	
	struct sockaddr_in ClientAddr;
	socklen_t ClientSize = sizeof(ClientAddr);

	socketClient = accept(socketFd, (struct sockaddr *)&ClientAddr, &ClientSize);
	std::cout << "client : " << socketClient << std::endl;


	close(socketClient);
	close(socketFd);
	return (0);
}
