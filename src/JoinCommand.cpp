#include "../includes/Server.hpp"
#include "../includes/JoinCommand.hpp"

void JoinCommand::execute(const std::vector<std::string>& args, int client_fd, Server& server)
{
	std::cout << "Server: JOIN ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
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
    Channel* targetChannel = server.getChannelByName(channelName);

    if (!targetChannel)
    {
        std::string errMsg = ":YourServer 403 " + channelName + " :No such channel";
        server.sendReply(errMsg, client_fd);
        return;
    }

    Client* client = server.getClientByFd(client_fd);

    // Vérifiez si le canal est en mode "invite-only" et si l'utilisateur a été invité
    if (targetChannel->has_mode(Channel::MODE_INVITE) && !targetChannel->isUserInvited(client))
    {
        std::string errMsg = ":YourServer 473 " + channelName + " :Cannot join channel (+i) - you must be invited";
        server.sendReply(errMsg, client_fd);
        return;
    }

    //ajoutez le client au canal
    targetChannel->addClient(client_fd, server);

	targetChannel->broadcastMessage(client->getNickname() + " has joined " + channelName, server);

}
