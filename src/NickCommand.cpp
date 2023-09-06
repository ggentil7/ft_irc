#include "../includes/NickCommand.hpp"

void NickCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (server.getClients()[client_fd]->getRegistration() == false)
	{
		server.sendReply(":ft_irc 451 :You have not registered", client_fd); // ERR_NOTREGISTERED
		server.sendReply("ERROR :Password required /connect localhost port password", client_fd);
		return;
	}
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 431 :No nickname given", client_fd); // ERR_NONICKNAMEGIVEN
		return;
	}

	std::string	oldNick = server.getClients()[client_fd]->getNickname();
	std::string newNick = args[0];

	if (server.isNickInUse(newNick))
	{
		server.sendReply(":ft_irc 433 " + newNick + " :Nickname is already in use", client_fd); // ERR_NICKNAMEINUSE
		return;
	}
	// If the nickname is valid and not in use, set it
	server.getClients()[client_fd]->setNickname(newNick);

	// Send a reply to confirm the change of nickname
	std::string	reply = ":" + oldNick + "!~username@host NICK :" + newNick;
	server.sendReply(reply, client_fd);
}
