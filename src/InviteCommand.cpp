#include "../includes/InviteCommand.hpp"

void InviteCommand::execute(const std::vector<std::string> &args, int socket, Server &server)
{
	std::cout << "Server: INVITE ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	(void)args;
	(void)socket;
	(void)server;
}
