// PassCommand.hpp
#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "ICommand.hpp"

class PassCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket);
};

#endif
