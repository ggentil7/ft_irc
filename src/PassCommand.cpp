#include "../includes/PassCommand.hpp"

void PassCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: PASS ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	//TODO

	if (args.size() < 2)
	{
		// Send error message for incorrect number of arguments
		return;
	}
	std::string password = args[1];
	(void) client_fd;
	(void) server;
	// Validate and associate the password with the client/connection
	// For example, update a password field in the associated Client object
}
