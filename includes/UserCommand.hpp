#ifndef USERCCOMMAND_HPP
#define USERCCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class UserCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
