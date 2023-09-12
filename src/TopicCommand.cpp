#include "../includes/TopicCommand.hpp"

void TopicCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 TOPIC :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string channelName = args[0];

	Channel	*channel = server.getChannelByName(channelName);
	Client	*client = server.getClientByFd(client_fd);

	if(!channel)
	{
		server.sendReply(":ft_irc 403 TOPIC :No such channel " + channelName, client_fd); // ERR_NOSUCHCHANNEL
		return;
	}
	if (!channel->isMember(client_fd))
	{
		server.sendReply(":ft_irc 442 TOPIC :You're not on that channel " + channelName, client_fd); // ERR_NOTONCHANNEL
		return;
	}
	if (args.size() == 1)
	{
		if (channel->getTopic().empty())
			server.sendReply(":ft_irc 331 " + client->getNickname() + " " + channelName + " :No topic is set", client_fd); // RPL_NOTOPIC
		else
			server.sendReply(":ft_irc 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic(), client_fd); // RPL_TOPIC
		return;
	}
	else if (args.size() == 2)
	{
		if (channel->isModeSet(Channel::TOPIC_PROTECTED) && !channel->isOperator(client_fd))
		{
			server.sendReply(":ft_irc 482 TOPIC :You're not channel operator", client_fd); // ERR_CHANOPRIVSNEEDED
			return;
		}
		std::string	newTopic = args[1];
		if (newTopic.empty())
		{
			channel->setTopic("");
			server.sendReply(":ft_irc 331 " + client->getNickname() + " " + channelName + " :No topic is set", client_fd); // RPL_NOTOPIC
		}
		else
		{
			channel->setTopic(newTopic);
			std::string	topicMsg = ":ft_irc 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic(); // RPL_TOPIC
			channel->broadcastMessage(topicMsg, server);
		}
	}
}
