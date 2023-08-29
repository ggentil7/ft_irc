#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <map>

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"

class Client;
class Channel;

class Server
{
private:
	std::map<int, Client*>			clients;
	std::map<std::string, Channel*>	channels;
	int								socket;

public:
	Server();
	Server(Server const &src);
	Server &operator=(Server const &rhs);
	~Server();

};

#endif
