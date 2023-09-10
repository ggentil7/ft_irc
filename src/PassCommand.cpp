#include "../includes/PassCommand.hpp"

void PassCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}
	if (server.getClients()[client_fd]->getRegistration() == Client::DONE)
	{
		server.sendReply(":ft_irc 462 " + server.getClients()[client_fd]->getNickname() + " :You may not reregister", client_fd); // ERR_ALREADYREGISTRED
		return;
	}
	if (args[0] != server.getPassword())
	{
		server.sendReply(":ft_irc 464 " + server.getClients()[client_fd]->getNickname() + " :Password incorrect", client_fd);
		server.sendReply("ERROR", client_fd);
	}
	else
	{
		server.getClients()[client_fd]->setRegistration(Client::PASS);
	}
}
