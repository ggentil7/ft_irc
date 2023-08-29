#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sstream>

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Command
{
public:
	Command(int client_fd, const std::string &message, std::map<int, Client> &clients);

	void	parse();
	void	dispatch();

private:
	int							_client_fd;
	std::string					_message;
	std::string					_command;
	std::vector<std::string>	_params;
	std::map<int, Client>&		_clients;

	std::map<std::string, void (Command::*)()>	_commandMap;

	void initCommandMap();

	void handleNick();
	void handleJoin();
	void handlePrivMsg();

	Command();
	Command(Command const &src);
	Command &operator=(Command const &rhs);
	~Command();
};

#endif
