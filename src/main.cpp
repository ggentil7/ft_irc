#include "../includes/Command.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

int	main(void)
{
	Server server; 

    server.setPort(PORT); 
    server.createSocket();
    server.connectionServer(); 
	
	return 0;
}
