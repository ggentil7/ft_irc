#include "../includes/PingCommand.hpp"

void PingCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}
	std::string	token = args[0];
	server.sendReply(":ft_irc PONG " + token, client_fd);
}
