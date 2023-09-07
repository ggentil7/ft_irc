#ifndef PINGCOMMAND_HPP
#define PINGCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class PingCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
