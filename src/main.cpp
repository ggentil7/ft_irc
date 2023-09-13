#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

int main(int argc,char **argv)
{
	Server	server(6667, "1234");

	if (argc == 3)
	{
		int	port = atoi(argv[1]);
		if (port < 1 || port > 65535)
		{
			std::cout << "Error: port out of range [1-65535]" << std::endl;
			return -1;
		}

		std::string	password(argv[2]);

		server.setPort(port);
		server.setPassword(password);
	}

	server.createSocket();
	server.connectionServer();

	return 0;
}
