#ifndef WHOISCOMMAND_HPP
#define WHOISCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class WhoisCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
