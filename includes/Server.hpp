#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <map>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
	std::map<std::string, Client*>	clients; //keyed by client nickname or ID (possible std::string a la palce de int)
	std::map<std::string, Channel*>	channels; //keyed by channel name
	int								_socket;
	int								_port;
	struct sockaddr_in				_addr;

public:
	Server();
	Server(int port);
	Server(Server const &src);
	Server &operator=(Server const &rhs);
	~Server();

	void setPort(int port);
	int	 getPort();
	int	 getSocket();

	void createSocket();
	void connectionServer();

};

#endif
