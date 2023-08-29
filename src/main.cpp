#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

int main(void)
{
	Server server;

	server.setPort(PORT);
	server.createSocket();
	server.connectionServer();

	return 0;
}
