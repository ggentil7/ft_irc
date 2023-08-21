#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>

class Client
{
private:
	int	socket;
	std::string	nickname;
	std::string	useername;
	std::string	hostname;
	std::string	realname;
	std::list<std::string>	channels;
public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

};

#endif
