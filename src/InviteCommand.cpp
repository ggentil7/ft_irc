#include "../includes/InviteCommand.hpp"
#include "../includes/Server.hpp"

void InviteCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "InviteCommand executed" << std::endl; // debug

	if (args.size() < 2)
	{
		server.sendReply(":server 461 " + server.getClients()[client_fd]->getNickname() + " INVITE :Not enough parameters", client_fd); //ERR_NEEDMOREPARAMS 
		return;
	}

	std::string targetNick = args[0];
	std::string channelName = args[1];

	// Check if the target client exists
	Client *targetClient = NULL;
	std::map<int, Client *>::iterator it;
	for (it = server.getClients().begin(); it != server.getClients().end(); ++it)
	{
		if (it->second && it->second->getNickname() == targetNick)
		{
			targetClient = it->second;
			break;
		}
	}

	if (!targetClient)
	{
		server.sendReply(":server 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick", client_fd); //ERR_NOSUCHNICK
		return;
	}

	// Check if the channel exists
	if (server.getChannels().find(channelName) == server.getChannels().end())
	{
		server.sendReply(":server 403 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :No such channel", client_fd); //ERR_NOTONCHANNEL
		return;
	}

	Channel *targetChannel = server.getChannels()[channelName];

    //check si le client est membre du channel
    if (!targetChannel->isMember(client_fd))
    {
        server.sendReply(":server 442 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :You're not on that channel", client_fd); //ERR_USERONCHANNEL
        return;
    }

	// Check if the channel is in INVITE_ONLY mode and if the client is an operator
    if (targetChannel->isModeSet(Channel::INVITE_ONLY) && !targetChannel->isOperator(client_fd))
    {
        server.sendReply(":server 482 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :You're not channel operator", client_fd); //ERR_CHANOPRIVSNEEDED
        return;
    }

    // Check if the target client is already a member of the channel
    if (targetChannel->isMember(targetClient->getFd()))
    {
        server.sendReply(":server 443 " + targetNick + " " + channelName + " :is already on channel", client_fd);
        return;
    }

	// Send the invite message to the target client
    server.sendReply(":" + server.getClients()[client_fd]->getNickname() + " INVITE " + targetNick + " :" + channelName, targetClient->getFd());

    // Notify the inviting user
    server.sendReply(":server 341 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " " + channelName, client_fd);

    // Add the target user to the list of invited users for the channel
    targetChannel->addInvitedUsers(targetClient);
}

