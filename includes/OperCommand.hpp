#ifndef OPERCOMMAND_HPP
#define OPERCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class OperCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
