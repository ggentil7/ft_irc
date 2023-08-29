#include "../includes/Server.hpp"

int	main(void)
{
	Server server; 

    server.setPort(PORT); 
    server.createSocket();
    server.connectionServer(); 
	
	return 0;
	
	return 0;
}
