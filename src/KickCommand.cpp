#include "../includes/KickCommand.hpp"

void KickCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 2)
	{
		server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string channelName = args[0];
	std::string targetNick = args[1];
	std::string kickReason;
			
	if (args.size() > 2) 
		 kickReason = args[2];
	
	Client	*kickingClient = server.getClientByFd(client_fd);
	Client	*targetClient = server.getClientByNickname(targetNick);
	Channel	*targetChannel = server.getChannelByName(channelName);

	if (!kickingClient->isModeSet(Client::OPERATOR))
	{
		server.sendReply(":ft_irc 482 MODE :You're not channel operator", client_fd);
		return;
	}
	if (targetChannel)
	{
		if (!targetChannel->isMember(client_fd))
		{
			server.sendReply(":ft_irc 442 MODE :You're not on that channel", client_fd); // ERR_NOTONCHANNEL
			return;
		}
		if ((!targetClient) || !targetChannel->isMember(targetClient->getFd()))
		{
			server.sendReply(":ft_irc 441 MODE :They aren't on that channel", client_fd); // ERR_USERNOTINCHANNEL
			return;
		}
		else
		{
				std::string reply = ":" + kickingClient->getNickname() + " KICK " + channelName +  " " + targetNick;
				
				if (kickReason.empty() == false)
					reply.append(" " + kickReason);

				targetChannel->broadcastMessage(reply, server); // ERR_USERNOTINCHANNEL
			
				targetChannel->removeMember(targetClient->getFd());
				return;
		}
	}
	else
	{
		server.sendReply(":ft_irc 403 MODE :No such channel " + channelName, client_fd); // ERR_NOSUCHCHANNEL
		return;
	}
}

//TODO : kick de plusieurs channels
