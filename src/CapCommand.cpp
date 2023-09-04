#include "../includes/CapCommand.hpp"
#include <vector>

void CapCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: CAP ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	if (args.size() > 2)
	{
		std::map<std::string, ICommand*> commandMap = server.getCommands();
		if (commandMap.find(args[2]) != commandMap.end())
					{
						ICommand *commandHandler =commandMap[args[2]];
						std::vector<std::string> newArgs(args.begin() + 3, args.end());
						commandHandler->execute(newArgs, client_fd, server);
					}
	}
	if (args[0] == "REQ" && args[1] == ":multi-prefix")
	{
		std::string capAckMsg = "CAP * ACK :multi-prefix";
		server.sendReply(capAckMsg, client_fd);
	}
}
