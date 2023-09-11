#include "../includes/NoticeCommand.hpp"

void NoticeCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 2)
		return;

	std::string	recipient = args[0];
	std::string	message;

	for (size_t i = 1; i < args.size(); ++i)
		message += " " + args[i];

	Client	*sender = server.getClients()[client_fd];
	if (!sender)
		return;

	server.sendMessage(recipient, message, sender);
}
