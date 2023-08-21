#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>

class Client
{
private:

public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

};

#endif
