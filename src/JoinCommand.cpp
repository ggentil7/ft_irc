#include "../includes/JoinCommand.hpp"

/*
	Syntax Validation: Make sure the client has sent a valid JOIN command according to the IRC protocol. The simplest form is JOIN #channelname.

	Authentication: Check if the user is authenticated or if the channel requires a special key (password).

	Channel Existence: Check if the channel already exists. If not, create it and add the client as the first member and probably also as the channel operator.

	Add User to Channel: If the channel exists, add the user to the channel's list of members.

	Send Notifications: Notify all members of the channel that a new user has joined.

	Send Reply: Finally, send a reply back to the client to acknowledge the JOIN.
*/

void JoinCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		std::string errMsg = ":ft_irc 461 JOIN :Not enough parameters"; // ERR_NEEDMOREPARAMS
		server.sendReply(errMsg, client_fd);
		return;
	}
}
