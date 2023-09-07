#include "../includes/InviteCommand.hpp"
#include "../includes/Server.hpp"

void InviteCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	Client	*invitingClient = server.getClientByFd(client_fd);

	if (args.size() < 2)
	{
		server.sendReply(":server 461 " + invitingClient->getNickname() + " INVITE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string targetNick = args[0];
	std::string channelName = args[1];

	Client	*targetClient = server.getClientByNickname(targetNick);
	Channel	*targetChannel = server.getChannelByName(channelName);

	if (!targetClient)
	{
		server.sendReply(":server 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick", client_fd); // ERR_NOSUCHNICK
		return;
	}

	if (targetChannel)
	{
		if (!targetChannel->isMember(client_fd))
		{
			server.sendReply(":server 442 " + invitingClient->getNickname() + " " + channelName + " :You're not on that channel", client_fd); // ERR_NOTONCHANNEL
			return;
		}
		if (targetChannel->isMember(targetClient->getFd()))
		{
			server.sendReply(":server 443 " + invitingClient->getNickname() + " " + targetNick + " " + channelName + " :is already on channel", client_fd);
			return;
		}
		if (targetChannel->isModeSet(Channel::INVITE_ONLY) && !invitingClient->isModeSet(Client::OPERATOR))
		{
			server.sendReply(":server 482 " + invitingClient->getNickname() + " " + channelName + " :You're not channel operator", client_fd);
			return;
		}
	}

	server.sendReply(":" + invitingClient->getNickname() + " INVITE " + targetNick + " :" + channelName, targetClient->getFd());

	targetChannel->addInvitedUser(targetClient);
}
