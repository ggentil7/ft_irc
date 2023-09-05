#include "../includes/Server.hpp"
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
	std::cout << "Server: JOIN ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	if (args.size() < 1)
	{
		std::string errMsg = ":ft_irc 461 JOIN :Not enough parameters"; // ERR_NEEDMOREPARAMS
		server.sendReply(errMsg, client_fd);
		return;
	}

	std::string channelName = args[0];
	Channel *targetChannel = server.getChannelByName(channelName);
	Client *client = server.getClientByFd(client_fd);

	if (!targetChannel) // si le channel existe pas
	{
		// creer le channel
		targetChannel = new Channel();
		server.addChannel(channelName, targetChannel);

		targetChannel->addClient(client_fd);
		targetChannel->addOperator(client_fd);

		std::string createMsg = "Channel " + channelName + " created.";
		server.sendReply(createMsg, client_fd);
	}

	else
	{
		// Vérifiez si le canal est en mode "invite-only" et si l'utilisateur a été invité
		if (targetChannel->isModeSet(Channel::INVITE_ONLY) && !targetChannel->isUserInvited(client))
		{
			std::string errMsg = ":YourServer 473 " + channelName + " :Cannot join channel (+i) - you must be invited";
			server.sendReply(errMsg, client_fd);
			return;
		}

		// ajoutez le client au canal
		targetChannel->addClient(client_fd);
	}

	targetChannel->broadcastMessage(client->getNickname() + " has joined " + channelName, server);
}
