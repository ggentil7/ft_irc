#include "../includes/InviteCommand.hpp"

void InviteCommand::execute(const std::vector<std::string> &args, int socket, Server &server)
{
	std::cout << "Server: INVITE ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

    (void) args;
    (void) socket;
    (void) server;
	
	// std::cout << "InviteCommand executed" << std::endl; //! debug
    // if (args.size() < 2)
    // {
    //     server.sendReply(":(nom du serveur) 461 " + server.getClients()[client_fd]->getNickname() + " INVITE :Not enough parameters", client_fd);
    //     return ;
    // }
    
    // std::string targetNick = args[0];
    // std::string channelName = args[1];

    // //s'assurer que le client visé existe
    // Client* targetClient = nullptr;
    // std::map<int, Client*>::iterator it;

    // for (it = server.getClients().begin(); it != server.getClients().end(); ++it)
    // {
    //     if (it->second->getNickname() == targetNick)
    //     {
    //         targetClient = it->second;
    //         break;
    //     }
    // }

    // if (!targetClient) {
    //     server.sendReply(":(nom du serveur) 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick/channel", client_fd);
    //     return;
    // }

}
