#include "../includes/InviteCommand.hpp"

void InviteCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: INVITE ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
	
	std::cout << "InviteCommand executed" << std::endl; //! debug

    if (args.size() < 2)
    {
        server.sendReply(":(nom du serveur) 461 " + server.getClients()[client_fd]->getNickname() + " INVITE :Not enough parameters", client_fd);
        return ;
    }
    //demande Nino quelle variable utilisé pour targetNick
    std::string targetNick = args[0];
    std::string channelName = args[1];
    //_channels = args[1];

    //s'assurer que le client visé existe
    Client* targetClient = nullptr;
    std::map<int, Client*>::iterator it;

    for (it = server.getClients().begin(); it != server.getClients().end(); ++it)
    {
        if (it->second->getNickname() == targetNick)
        {
            targetClient = it->second;
            break;
        }
    }

    if (!targetClient) {
        server.sendReply(":(nom du serveur) 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick or channel", client_fd);
        return;
    }

    //s'assurer que le channel existe
    if (server.getChannel().find(channelName) == server.getChannel().end())
    {
        server.sendReply(":(nom du serveur) 403" + server.getClients()[client_fd]->getNickname() + " " + channelName + " :No such channel", client_fd);
        return ;
    }

    server.sendReply(":" + server.getClients()[client_fd]->getNickname() + "INVITE: " + targetNick + " : " + channelName, targetClient->getFd());
    
    server.sendReply(":(nom du serveur) 341" + server.getClients()[client_fd]->getNickname() + " " + targetNick + " " + channelName, client_fd);

}
