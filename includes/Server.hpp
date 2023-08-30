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
#include <sstream>
#include <cstdlib>

#define IP_SERVER "127.0.0.1"
#define BACK_LOG 30
#define PORT 6667

#include "Channel.hpp"
#include "Client.hpp"
#include "ICommand.hpp"
#include "JoinCommand.hpp"
#include "NickCommand.hpp"
#include "PassCommand.hpp"

class Client;
class Channel;
class ICommand;

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
	std::map<std::string, ICommand*>	commandMap;

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

	std::pair<std::string, std::vector<std::string> >	parse(std::string message);
};

#endif
