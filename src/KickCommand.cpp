#include "../includes/KickCommand.hpp"

void KickCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 2)
	{
		server.sendReply(":ft_irc 461 KICK :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string channelName = args[0];
	std::string targetNick = args[1];
	std::string kickReason;
			
	if (args.size() > 2) 
		 kickReason = args[2];

	Client	*kickingClient = server.getClientByFd(client_fd);
	Channel	*targetChannel = server.getChannelByName(channelName);
	Client	*targetClient = server.getClientByNickname(targetNick);

	if (targetChannel)
	{
		if (!targetChannel->isMember(client_fd))
		{
			server.sendReply(":ft_irc 442 " + kickingClient->getNickname() + " " + channelName +  " :You're not on that channel", client_fd); // ERR_NOTONCHANNEL
			return;
		}
		if ((!targetClient) || !targetChannel->isMember(targetClient->getFd()))
		{
			server.sendReply(":ft_irc 441 " + kickingClient->getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel", client_fd); // ERR_USERNOTINCHANNEL
			return;
		}
		if ((!kickingClient) || !targetChannel->isOperator(kickingClient->getFd()))
		{
			server.sendReply(":ft_irc 482 " + kickingClient->getNickname() + " " + channelName + " :You're not channel operator", client_fd); // ERR_CHANOPRIVSNEEDED
			return;
		}

		std::string reply = ":" + kickingClient->getNickname() + " KICK " + channelName +  " " + targetNick;
		
		if (kickReason.empty() == false)
			reply.append(" " + kickReason);

		targetChannel->broadcastMessage(reply, server);
	
		targetChannel->removeMember(targetClient->getFd());
	}
	else
		server.sendReply(":ft_irc 403 " + kickingClient->getNickname() + " " + channelName +  " :No such channel", client_fd); // ERR_NOSUCHCHANNEL
}
