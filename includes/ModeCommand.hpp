#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class ModeCommand : public ICommand
{
public:
	void	execute(const std::vector<std::string> &args, int socket, Server& server);
	int		parseChannelMode(const std::string &modeStr);
	int		parseClientMode(const std::string &modeStr);
};

#endif
