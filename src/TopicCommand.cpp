#include "../includes/TopicCommand.hpp"

void TopicCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string target;

	if (args[0][0] == '#')
		target = args[0];
	else
	{
		target = "#" + args[0];
	}
		Channel *channel = server.getChannelByName(target);
		Client	*client = server.getClientByFd(client_fd);

		if(!channel)
		{
			server.sendReply(":ft_irc 403 MODE :No such channel " + target, client_fd); // ERR_NOSUCHCHANNEL
			return;
		}
		if (!channel->isMember(client_fd))
		{
			server.sendReply(":ft_irc 442 MODE :You're not on that channel " + target, client_fd); // ERR_CHANOPRIVSNEEDED
			return;
		}
		if ((channel->isModeSet(Channel::TOPIC_PROTECTED)) && (!client->isModeSet(Client::OPERATOR)))
		{
			server.sendReply(":ft_irc 482 MODE :You're not channel operator", client_fd); // ERR_NOTONCHANNEL
			return;
		}
		else
		{	
			std::string newTopic;
			for (size_t i = 1; i < args.size(); i++)
				newTopic.append(args[i]);
			channel->setTopic(newTopic);

			std::string reply = ":" + server.getClients()[client_fd]->getNickname() + " TOPIC " + channel->getName() + " :" + channel->getTopic();
			channel->broadcastMessage(reply, server);
		}
}
