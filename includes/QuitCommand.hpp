#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class QuitCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
