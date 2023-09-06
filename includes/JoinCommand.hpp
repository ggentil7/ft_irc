#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class Server;

class JoinCommand : public ICommand {
public:
	virtual ~JoinCommand() {}

	void execute(const std::vector<std::string>& args, int socket, Server& server);
};

#endif
