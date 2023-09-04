#ifndef CAPCOMMAND_HPP
#define CAPCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class CapCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
