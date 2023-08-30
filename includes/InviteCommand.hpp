#ifndef INVITECOMMAND_HPP
#define INVITECOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class Server;

class InviteCommand : public ICommand {
public:
	virtual ~InviteCommand() {}

	void execute(const std::vector<std::string>& args, int socket, Server& server);
};

#endif 
