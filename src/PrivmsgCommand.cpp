#include "../includes/PrivmsgCommand.hpp"

void PrivmsgCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::cout << "Server: PRIVMSG ";
	std::vector<std::string> newArgs(args);
	for (std::vector<std::string>::iterator it = newArgs.begin(); it != newArgs.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	if (args.size() < 1)
	{
		server.sendReply(":No recipient given (PRIVMSG)", client_fd);
		return;
	}
	if (args.size() < 2)
	{
		server.sendReply(":No text to send", client_fd);
		return;
	}
	std::string recipient = args[0];
	std::string message = args[1];
	for (size_t i = 3; i < args.size(); ++i)
	{
		message += " " + args[i];
	}
	std::cout << "recipient = " << recipient << std::endl;
	std::cout << "message = " << message << std::endl;
	Client *sender = server.getClients()[client_fd];
	std::cout << "sender nickname = " << sender->getNickname() << std::endl;
	if (sender == NULL)
	{
		// The sender is not in the server's client list, handle error
		return;
	}

	// Use the sendMessage function from the Server class
	bool success = server.sendMessage(recipient, message, sender);
	if (!success)
	{
		// The recipient does not exist, handle error
		std::cout << "recipient does not exist" << std::endl;
	}
}
