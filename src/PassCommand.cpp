#include "../includes/PassCommand.hpp"

void PassCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}
	if (server.getClients()[client_fd]->getRegistration() == true)
	{
		server.sendReply(":ft_irc 462 " + server.getClients()[client_fd]->getNickname() + " :You may not reregister", client_fd); // ERR_ALREADYREGISTRED
		return;
	}
	if (args[0] != server.getPassword())
		server.sendReply(":ft_irc 464 " + server.getClients()[client_fd]->getNickname() + " :Password incorrect", client_fd);
	else
	{
		server.getClients()[client_fd]->setRegistration(true);

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

		// Send CAP LS
		std::string capLSMsg = ":ft_irc CAP * LS :multi-prefix";
		server.sendReply(capLSMsg, client_fd);

		// Send MOTD
		std::string motdMsg = ":ft_irc 372 " + server.getClients()[client_fd]->getNickname() + " :- Welcome to ft_irc!";
		server.sendReply(motdMsg, client_fd);

		std::string endMOTDMsg = ":ft_irc 376 " + server.getClients()[client_fd]->getNickname() + " :End of MOTD command";
		server.sendReply(endMOTDMsg, client_fd);

		// Send Initial Modes
		std::string modeAckMsg = ":ft_irc 221 " + server.getClients()[client_fd]->getNickname() + " +i";
		server.sendReply(modeAckMsg, client_fd);
	}
}
