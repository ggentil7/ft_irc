#include "../includes/JoinCommand.hpp"
#include "../includes/Server.hpp"

void JoinCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		std::string errMsg = ":ft_irc 461 JOIN :Not enough parameters"; // ERR_NEEDMOREPARAMS
		server.sendReply(errMsg, client_fd);
		return;
	}

	std::string channelName;
	if (args.size() == 1 && args[0] == "")
		channelName = "ft_irc";
	else
		channelName = args[0];

	if (channelName[0] != '#')
		return;

	Channel *targetChannel = server.getChannelByName(channelName);
	Client *client = server.getClientByFd(client_fd);

	if (!targetChannel) // si le channel existe pas
	{
		// creer le channel
		targetChannel = new Channel();
		server.addChannel(channelName, targetChannel);

		targetChannel->addClient(client_fd);
		targetChannel->addOperator(client_fd);

		client->setMode(Client::OPERATOR, true);

		std::string createMsg = "Channel " + channelName + " created.";
		server.sendReply(createMsg, client_fd);
	}
	else
	{
		// Check if user limit is set and if channel is full
		if (targetChannel->getUserLimit() > 0 && targetChannel->getMembers().size() >= targetChannel->getUserLimit())
		{
			server.sendReply(":ft_irc 471 " + server.getClients()[client_fd]->getNickname() + " " +  channelName + " :Cannot join channel (+l) - channel is full", client_fd);
			return;
		}
		// Vérifiez si le canal est en mode "invite-only" et si l'utilisateur a été invité
		if (targetChannel->isModeSet(Channel::INVITE_ONLY) && !targetChannel->isUserInvited(client))
		{
			std::string errMsg = ":ft_irc 473 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :Cannot join channel (+i) - you must be invited"; // ERR_INVITEONLYCHAN
			server.sendReply(errMsg, client_fd);
			return;
		}
		// ajoutez le client au canal
		targetChannel->addClient(client_fd);
	}
	targetChannel->broadcastMessage(client->getNickname() + " has joined " + channelName, server);
}
