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
#include "InviteCommand.hpp"

class Client;
class Channel;
class ICommand;

class Server
{
private:
	int									_port;
	std::string							_password;
	int									_socket;
	bool								_validPassword;
	struct sockaddr_in					_addr;
	std::vector<int> 					_client_socket;
	std::map<int, Client*>				_clients;		// keyed by fd
	std::map<std::string, Channel*>		_channels;		//keyed by channel name
	std::map<std::string, ICommand*>	_commandMap;	// keyed by command name

public:
	Server();
	Server(int port, std::string password);
	Server(Server const &src);
	Server &operator=(Server const &src);
	~Server();

	void 		setPort(int port);
	int	 		getPort();
	int	 		getSocket();

	std::map<int, Client*>				getClients();
	std::map<std::string, Channel*>		getChannels();
	std::map<std::string, ICommand*>	getCommands();

	Channel	*getChannelByName(const std::string name);
	Client	*getClientByNickname(const std::string nickname);
	Client	*getClientByFd(int fd);

	std::string getPassword();
	void		setPassword(std::string password);
	bool		getValidPassword();

	void 		createSocket();
	void 		connectionServer();

	std::pair<std::string, std::vector<std::string> >	parse(std::string message);

	bool	isNickInUse(const std::string& nick);
	void	sendReply(const std::string& message, int client_fd);
	bool	sendMessage(const std::string &recipient, const std::string &message, Client *sender);
};

#endif
