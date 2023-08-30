#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "ICommand.hpp"
#include <iostream>

class ModeCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
