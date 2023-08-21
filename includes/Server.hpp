#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>

class Server
{
private:

public:
	Server();
	Server(Server const &src);
	Server &operator=(Server const &rhs);
	~Server();

};

#endif
