#ifndef NOTCICECOMMAND_HPP
#define NOTCICECOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class NoticeCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
