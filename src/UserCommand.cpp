#include "../includes/UserCommand.hpp"

void UserCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	(void) args;

	Client	*client = server.getClients()[client_fd];

	if (client && client->getRegistration() == Client::NICK)
	{
		// Welcome Message (RPL_WELCOME)
		std::string welcomeMsg = ":ft_irc 001 " + client->getNickname() + " :Welcome to the IRC Network " + client->getNickname();
		server.sendReply(welcomeMsg, client_fd);

		// Your Host (RPL_YOURHOST)
		std::string yourHostMsg = ":ft_irc 002 " + client->getNickname() + " :Your host is ft_irc, running version ircd-2.10.3"; //? use config file
		server.sendReply(yourHostMsg, client_fd);

		// Send Server Created
		std::string createdMsg = ":ft_irc 003 " + client->getNickname() + " :This server was created {TODO}"; //! add real time ?
		server.sendReply(createdMsg, client_fd);

		// Send My Info
		std::string myInfoMsg = ":ft_irc 004 " + client->getNickname() + " ft_irc ircd-2.10.3"; //? use config file
		server.sendReply(myInfoMsg, client_fd);

		client->setRegistration(Client::DONE);

		if (server.getClients().size() == 1)
		{
			client->setMode(Client::OPERATOR, true);
			server.sendReply(":ft_irc 221 " + client->getNickname() + " :+o", client_fd);
		}
	}
}
