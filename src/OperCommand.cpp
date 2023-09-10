#include "../includes/OperCommand.hpp"

void OperCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 2)
	{
		server.sendReply(":server 461 OPER :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string username = args[0];
	std::string password = args[1];

	Client *client = server.getClientByFd(client_fd);

	if (!client)
		return;

	if (username == "nick" && password == "password") //? use config file
	{
		client->setMode(Client::OPERATOR, true);
		server.sendReply(":server 381 " + client->getNickname() + " :You are now an IRC operator", client_fd); // RPL_YOUREOPER
	}
	else
		server.sendReply(":server 464 " + client->getNickname() + " :Nick/Password incorrect", client_fd); // ERR_PASSWDMISMATCH
}
