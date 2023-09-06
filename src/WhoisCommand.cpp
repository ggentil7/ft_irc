#include "../includes/WhoisCommand.hpp"

void WhoisCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	(void) args;
	std::string whoisMsg = ":YourServer 311 " + server.getClients()[client_fd]->getNickname() + " " + server.getClients()[client_fd]->getUsername() + " " + server.getClients()[client_fd]->getHostname() + " * :" + server.getClients()[client_fd]->getRealname();
	server.sendReply(whoisMsg, client_fd);

	return;
}
