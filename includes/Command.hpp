#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sstream>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Command
{
public:
	Command();
	Command(int client_fd, const std::string &message);
	Command(Command const &src);
	Command &operator=(Command const &rhs);
	~Command();

	void	parse();
	void	dispatch();

private:
	int							_client_fd;
	std::string					_message;
	std::string					_command;
	std::vector<std::string>	_params;

	std::map<std::string, void (Command::*)()>	_commandMap;

	void initCommandMap();

	void handleNick();
	void handleJoin();
	void handlePrivMsg();
};

#endif
