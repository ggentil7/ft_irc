#include "../includes/WhoisCommand.hpp"

void WhoisCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: WHOIS ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	std::string whoisMsg = ":YourServer 311 " + server.getClients()[client_fd]->getNickname() + " " + server.getClients()[client_fd]->getUsername() + " " + server.getClients()[client_fd]->getHostname() + " * :" + server.getClients()[client_fd]->getRealname();
	server.sendReply(whoisMsg, client_fd);

	return;
}
