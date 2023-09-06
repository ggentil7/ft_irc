#include "../includes/PrivmsgCommand.hpp"

void PrivmsgCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 411 :No recipient given (PRIVMSG)", client_fd); // ERR_NORECIPIENT
		return;
	}
	if (args.size() < 2)
	{
		server.sendReply(":ft_irc 412 :No text to send", client_fd); // ERR_NOTEXTTOSEND
		return;
	}
	std::string recipient = args[0];
	std::string message;

	for (size_t i = 1; i < args.size(); ++i)
	{
		message += " " + args[i];
	}
	Client *sender = server.getClients()[client_fd];
	if (sender == NULL)
	{
		server.sendReply("ERROR", client_fd);
		return;
	}
	bool success = server.sendMessage(recipient, message, sender);
	if (!success)
		server.sendReply(":ft_irc 401 " + recipient + " :No such nick/channel", client_fd); // ERR_NOSUCHNICK
}
