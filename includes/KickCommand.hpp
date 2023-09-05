#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class KickCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
