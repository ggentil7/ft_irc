#include "../includes/JoinCommand.hpp"

void JoinCommand::execute(const std::vector<std::string>& args, int client_fd, Server& server)
{
	std::cout << "Server: JOIN ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	(void) server;
	if (args.size() < 1)
	{
		std::string errMsg = ":YourServer 461 JOIN :Not enough parameters";
		server.sendReply(errMsg, client_fd);
		return;
	}
}
