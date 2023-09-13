#include "../includes/UserCommand.hpp"

void UserCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	Client	*client = server.getClients()[client_fd];

	if (client && client->getRegistration() == Client::NICK)
	{
		if (args.size() < 4)
		{
			server.sendReply(":ft_irc 461 " + client->getNickname() +  " USER :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
			return;
		}

		client->setUsername(args[0]);
		// client->setMode(atoi(args[1].c_str()), true);
		client->setHostname(args[2]);
		client->setRealname(args[3]);

		std::string welcomeMsg = ":ft_irc 001 " + client->getNickname() + " :Welcome to the IRC Network, " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname(); // RPL_WELCOME
		server.sendReply(welcomeMsg, client_fd);

		std::string yourHostMsg = ":ft_irc 002 " + client->getNickname() + " :Your host is ft_irc, running version 1.0"; // RPL_YOURHOST
		server.sendReply(yourHostMsg, client_fd);

		std::string createdMsg = ":ft_irc 003 " + client->getNickname() + " :This server was created today"; // RPL_CREATED
		server.sendReply(createdMsg, client_fd);

		std::string myInfoMsg = ":ft_irc 004 " + client->getNickname() + " ft_irc 1.0"; // RPL_MYINFO
		server.sendReply(myInfoMsg, client_fd);

		client->setRegistration(Client::DONE);

		if (server.getClients().size() == 1)
		{
			client->setMode(Client::OPERATOR, true);
			server.sendReply(":ft_irc 221 " + client->getNickname() + " :+o", client_fd);
		}
	}
	else if (client && client->getRegistration() == Client::DONE)
	{ 
		server.sendReply(":ft_irc 462 " + client->getNickname() + " :You may not reregister", client_fd);
		return;
	}
}
