#include "../includes/InviteCommand.hpp"
#include "../includes/Server.hpp"

// void InviteCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
// {
// 	std::cout << "Server: INVITE ";
// 	std::vector<std::string> newArgs(args);
// 	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
// 	{
// 		std::cout << *it << ' ';
// 	}
// 	std::cout << std::endl;
	
// 	std::cout << "InviteCommand executed" << std::endl; //! debug

//     if (args.size() < 2)
//     {
//         server.sendReply("ERR_NEEDMOREPARAMS 461 " + server.getClients()[client_fd]->getNickname() + " INVITE :Not enough parameters", client_fd);
//         return ;
//     }

//     std::string targetNick = args[0];
//     std::string channelName = args[1];

//     //s'assurer que le client visé existe
//     Client* targetClient = nullptr;
//     std::map<int, Client*>::iterator it;

//     for (it = server.getClients().begin(); it != server.getClients().end(); ++it)
//     {
//         if (it->second->getNickname() == targetNick)
//         {
//             targetClient = it->second;
//             break;
//         }
//         if (!targetClient) 
//         {
//             server.sendReply("ERR_NOSUCHNICK 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick", client_fd);
//             return;
//         }

//         //s'assurer que le channel existe
//         if (server.getChannel().find(channelName) == server.getChannel().end())
//         {
//             server.sendReply("ERR_NOTONCHANNEL 403" + server.getClients()[client_fd]->getNickname() + " " + channelName + " :No such channel", client_fd);
//             return ;
//         }
//         //previens l'utilisateur de l'invitatin au channel
//         server.sendReply(":" + server.getClients()[client_fd]->getNickname() + "INVITE: " + targetNick + " : " + channelName, targetClient->getFd());
        
//         server.sendReply(":(nom du serveur) 341" + server.getClients()[client_fd]->getNickname() + " " + targetNick + " " + channelName, client_fd);
//     }

// }


void InviteCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: INVITE ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
	
	std::cout << "InviteCommand executed" << std::endl; // debug

   
     if (args.size() < 2)
    {
        server.sendReply(":server 461 " + server.getClients()[client_fd]->getNickname() + " INVITE :Not enough parameters", client_fd);
        return;
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    // Check if the target client exists
    Client* targetClient = nullptr;
    std::map<int, Client*>::iterator it;
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
        // Send an error message if the target client doesn't exist
        server.sendReply(":server 401 " + server.getClients()[client_fd]->getNickname() + " " + targetNick + " :No such nick/channel", client_fd);
        return;
    }

    // Check if the channel exists
    if (server.getChannel().find(channelName) == server.getChannel().end())
    {
        // Send an error message if the channel doesn't exist
        server.sendReply(":server 403 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :No such channel", client_fd);
        return;
    }

    Channel* targetChannel = server.getChannel()[channelName];

    if (targetChannel && !targetChannel->has_mode(Channel::MODE_INVITE))
    {
        server.sendReply(":server 442 " + server.getClients()[client_fd]->getNickname() + " " + channelName + " :You're not on that channel", client_fd);
        return;
    }

    // Send the invite message to the target client
    server.sendReply(":" + server.getClients()[client_fd]->getNickname() + " INVITE " + targetNick + " :" + channelName, targetClient->getFd());

    //ajouter le user cible a la liste des utilisateurs invité au channel
    targetChannel->addInvitedUsers(targetClient);
}
