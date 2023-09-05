#ifndef TOPICCOMMAND_HPP
#define TOPICCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class TopicCommand : public ICommand
{
public:
	void execute(const std::vector<std::string> &args, int socket, Server& server);
};

#endif
