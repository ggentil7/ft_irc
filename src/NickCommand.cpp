#include "../includes/NickCommand.hpp"

void NickCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 431 :No nickname given", client_fd); // ERR_NONICKNAMEGIVEN
		return;
	}

	if (server.getClients()[client_fd]->getRegistration() == Client::FAIL)
	{
		server.sendReply(":ft_irc 451 :You have not registered", client_fd); // ERR_NOTREGISTERED
		return;
	}

	std::string	oldNick = server.getClients()[client_fd]->getNickname();
	std::string newNick = args[0];

	if (server.getClients()[client_fd]->getRegistration() == Client::DONE)
	{
		if (server.isNickInUse(newNick))
			server.sendReply(":ft_irc 433 " + newNick + " :Nickname is already in use", client_fd); // ERR_NICKNAMEINUSE
		else
		{
			server.getClients()[client_fd]->setNickname(newNick);
			std::string	reply = ":" + oldNick + "!~username@host NICK :" + newNick;
			server.sendReply(reply, client_fd);
		}
		return;
	}

	if (server.isNickInUse(newNick))
	{
		std::string			defaultNickBase = newNick;
		std::string			defaultNick;
		std::ostringstream	convert;

		int uniqueID = client_fd;
		do	{
				convert.str("");							// Clearing the stringstream
				convert.clear();							// Clearing the stringstream state flags
				convert << newNick << uniqueID++;	// increment uniqueID after using its value
				newNick = convert.str();
			}
		while	(server.isNickInUse(newNick));
	}

	server.getClients()[client_fd]->setNickname(newNick);

	// Send a reply to confirm the change of nickname
	std::string	reply = ":" + oldNick + "!~username@host NICK :" + newNick;
	server.sendReply(reply, client_fd);

	// Welcome Message (RPL_WELCOME)
	std::string welcomeMsg = ":ft_irc 001 " + server.getClients()[client_fd]->getNickname() + " :Welcome to the IRC Network " + server.getClients()[client_fd]->getNickname();
	server.sendReply(welcomeMsg, client_fd);

	// Your Host (RPL_YOURHOST)
	std::string yourHostMsg = ":ft_irc 002 " + server.getClients()[client_fd]->getNickname() + " :Your host is ft_irc, running version ircd-2.10.3"; //? use config file
	server.sendReply(yourHostMsg, client_fd);

	// Send Server Created
	std::string createdMsg = ":ft_irc 003 " + server.getClients()[client_fd]->getNickname() + " :This server was created {TODO}"; //! add real time ?
	server.sendReply(createdMsg, client_fd);

	// Send My Info
	std::string myInfoMsg = ":ft_irc 004 " + server.getClients()[client_fd]->getNickname() + " ft_irc ircd-2.10.3"; //? use config file
	server.sendReply(myInfoMsg, client_fd);

	server.getClients()[client_fd]->setRegistration(Client::DONE);
}
