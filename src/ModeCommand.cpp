#include "../includes/ModeCommand.hpp"

void ModeCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: MODE ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	//! TODO
	(void) args;
	(void) client_fd;
	(void) server;
	return;
}

