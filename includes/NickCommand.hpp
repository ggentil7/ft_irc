#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class Server;

class NickCommand : public ICommand {
public:
	virtual ~NickCommand() {}

	void execute(const std::vector<std::string>& args, int socket, Server& server);
};

#endif // NICKCOMMAND_HPP
