#include "../includes/InviteCommand.hpp"

void InviteCommand::execute(const std::vector<std::string>& args, int socket, Server& server)
{
    std::cout << "InviteCommand executed" << std::endl; //! debug
    if (args.size() < 2)
    {
        server.sendReply("ERR_NEEDMOREPARAMS :Need more params", client_fd);
        return ;
    }
    std::string nickname = server.getClients()[client_fd]->getNickname();
    // std::string channel_name = server.getChannel()[client_fd]
    //voir comment faire pour choper le nom de channel et le filer args[1]

    
}
