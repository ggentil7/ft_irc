#include "Server.hpp"

int	main(void)
{
	Server server; 

    // server.setPort(atoi(argv[1]));

    server.setPort(8085); 
    server.createSocket();
    server.connectionServer(); 
	
	return 0;
}
