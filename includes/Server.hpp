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
#include <cstring> 
#include <cerrno>
#include <vector>

#define IP_SERVER "127.0.0.1"
#define BACK_LOG 30
#define PORT 8085


#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"

class Client;
class Channel;

class Server
{
private:
	std::map<std::string, Client*>	clients; //keyed by client nickname or ID
	std::map<std::string, Channel*>	channels; //keyed by channel name
	int								_port;
	int								_socket;
	bool							_validPassword;
	struct sockaddr_in				_addr;
	std::string						_password;
	std::vector<int> 				client_socket;

public:
	Server();
	// Server(int port, std::string password);
	Server(Server const &src);
	Server &operator=(Server const &src);
	~Server();

	void 		setPort(int port);
	int	 		getPort();
	int	 		getSocket();

	std::string getPassword();
	bool		getValidPassword();

	void 		createSocket();
	void 		connectionServer();

};

#endif
