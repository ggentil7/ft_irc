#include "../includes/QuitCommand.hpp"

void QuitCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	std::string quitMessage = "Client Quit";
	if (args.size() >= 1)
		quitMessage = args[0];

	// Fetch the client who is quitting
	Client	*quittingClient = server.getClients()[client_fd];
	if (!quittingClient)
		return;

	// Notify all channels that the client was part of
	std::map<std::string, Channel *> channels = server.getChannels();
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel *channel = it->second;
		if (channel->isMember(client_fd))
		{
			// Remove the client from the channel
			channel->removeMember(client_fd);

			// Notify the channel
			std::string notification = ":" + quittingClient->getNickname() + " QUIT :" + quitMessage;
			channel->broadcastMessage(notification, server);
		}
	}
}
