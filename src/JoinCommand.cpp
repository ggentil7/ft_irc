#include "../includes/JoinCommand.hpp"

void JoinCommand::execute(const std::vector<std::string>& args, int client_fd, Server& server)
{
	if (args.size() < 1)
	{
		std::string errMsg = ":ft_irc 461 JOIN :Not enough parameters"; // ERR_NEEDMOREPARAMS
		server.sendReply(errMsg, client_fd);
		return;
	}
}
