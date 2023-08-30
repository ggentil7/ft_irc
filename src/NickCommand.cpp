#include "../includes/NickCommand.hpp"

void NickCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "NickCommand executed" << std::endl; //! debug
	if (args.size() < 1)
	{
		server.sendReply("ERR_NONICKNAMEGIVEN :No nickname given", client_fd);
		return;
	}

	std::string	oldNick = server.getClients()[client_fd]->getNickname();
	std::string newNick = args[0];

	// Check if the nickname is already in use
	if (server.isNickInUse(newNick))
	{
		server.sendReply("ERR_NICKNAMEINUSE " + newNick + " :Nickname is already in use.", client_fd);
		return;
	}
	// If the nickname is valid and not in use, set it
	server.getClients()[client_fd]->setNickname(newNick);

	// Send a reply to confirm the change of nickname
	std::string	reply = ":" + oldNick + "!~username@host NICK :" + newNick + "\r\n";
	server.sendReply(reply, client_fd);
}
