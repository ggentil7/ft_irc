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

	std::string	channelName = args[0];

	if (channelName[0] != '&' && channelName[0] != '#' && channelName[0] != '+' && channelName[0] != '!')
		return;

	Client	*client = server.getClientByFd(client_fd);
	if (!client)
		return;

	std::string	joinMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN :" + channelName;

	Channel	*targetChannel = server.getChannelByName(channelName);
	if (!targetChannel)
	{
		targetChannel = new Channel();
		server.addChannel(channelName, targetChannel);

		targetChannel->addMember(client_fd);
		targetChannel->broadcastMessage(joinMsg, server);

		targetChannel->addOperator(client_fd);
		server.sendReply(":" + client->getNickname() + " MODE " + channelName + " :+o " + client->getNickname(), client_fd);
	}
	else
	{
		if (targetChannel->getUserLimit() > 0 && targetChannel->getMembers().size() >= targetChannel->getUserLimit())
		{
			server.sendReply(":ft_irc 471 " + server.getClients()[client_fd]->getNickname() + " " +  channelName + " :Cannot join channel (+l) - channel is full", client_fd); // ERR_CHANNELISFULL
			return;
		}
		if (targetChannel->isModeSet(Channel::INVITE_ONLY) && !targetChannel->isUserInvited(client))
		{
			std::string errMsg = ":ft_irc 473 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :Cannot join channel (+i) - you must be invited"; // ERR_INVITEONLYCHAN
			server.sendReply(errMsg, client_fd);
			return;
		}
		if (targetChannel->isModeSet(Channel::KEY_PROTECTED))
		{
			if (args.size() < 2 || args[1] != targetChannel->getKey())
			{
				server.sendReply(":server 475 " + client->getNickname() + " " + channelName + " :Cannot join channel (+k)", client_fd);
				return;
			}
		}
		targetChannel->addMember(client_fd);
		targetChannel->broadcastMessage(joinMsg, server);
	}

	if (!targetChannel->getTopic().empty())
			server.sendReply(":ft_irc 332 " + client->getNickname() + " " + channelName + " :" + targetChannel->getTopic(), client_fd); // RPL_TOPIC

	std::string rplNamReply = ":ft_irc 353 " + server.getClientByFd(client_fd)->getNickname() + " = " + channelName + " :";

	std::list<int> members = targetChannel->getMembers();

	for (std::list<int>::iterator it = members.begin(); it != members.end(); ++it)
	{
		Client *client = server.getClientByFd(*it);
		std::string prefix = "";
		if (targetChannel->isOperator(*it))
			prefix = "@";
		rplNamReply += prefix + client->getNickname() + " ";
	}

	server.sendReply(rplNamReply, client_fd); // RPL_NAMREPLY
	server.sendReply(":ft_irc 366 " + server.getClientByFd(client_fd)->getNickname() + " " + channelName + " :End of /NAMES list", client_fd); // RPL_ENDOFNAMES
}
