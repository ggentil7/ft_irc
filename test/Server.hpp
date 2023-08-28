#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstring>  // pour strlen et autres fonctions de chaînes
#include <cerrno>
#include <vector>
// #include "../includes/Client.hpp"
// #include "../includes/Channel.hpp"


class Server
{
private:
	// std::map<std::string, Client*>	clients; //keyed by client nickname or ID (possible std::string a la palce de int)
	// std::map<std::string, Channel*>	channels; //keyed by channel name
	int								_port;
	int								_socket;
	struct sockaddr_in				_addr;
	std::vector<int> client_socket;

public:
	Server();
	// Server(int port);
	Server(Server const &src);
	Server &operator=(Server const &src);
	~Server();

	void setPort(int port);
	int	 getPort();
	int	 getSocket();

	void createSocket();
	void connectionServer();

};

#endif
