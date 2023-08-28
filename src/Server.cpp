#include "../includes/Server.hpp"

Server::Server() : _port(0), _socket(0)
{
}

// Server::Server(int port)
// {
// }

Server::Server(Server const &src)
{
    *this = src;
}

Server &Server::operator=(Server const &src)
{
    _socket = src._socket;
    _port = src._port;
    _addr = src._addr;

    return (*this);
}

Server::~Server()
{
}

void Server::setPort(int port)
{
    _port = port;
}

int Server::getPort()
{
    return (_port);
}

int Server::getSocket()
{
    return (_socket);
}

void Server::createSocket()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);

    if (bind(_socket, (struct sockaddr*)&_addr, sizeof(_addr)) < 0)
    {
        std::cerr << "bind failed" << std::endl;;
        exit(EXIT_FAILURE);
    }

    if (listen(_socket, 10) < 0)
    {
        std::cerr << "listen failed" << std::endl;;
        exit(EXIT_FAILURE);
    }
}

void Server::connectionServer()
{
    int new_socket_client;
    int max_clients = 30;
    int client_socket[30];
    int valread;
    socklen_t addrlen = sizeof(_addr);
    char buffer[1025];
    //std::map<int, std::string> message;

    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(_socket, &readfds);
        int max_sd = _socket; //représente le maximum socket descriptor

         for (int i = 0; i < max_clients; i++)
         {
            if (client_socket[i] > 0)
            {
                FD_SET(client_socket[i], &readfds);
                if (client_socket[i] > max_sd)
                {
                    max_sd = client_socket[i];
                }
            }
         }

         //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
         int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

         if ((activity < 0) && (errno != EINTR)) //vérifie s'il n'y a pas d'erreur lors de l'appel à select() et si l'erreur n'est pas du à une interruption de signal (EINTR)
         {
            std::cerr << "select() error" << std::endl;
         }
         if (FD_ISSET(_socket, &readfds))
         {
            if ((new_socket_client = accept(_socket, (struct sockaddr*)&_addr, &addrlen)) < 0)
            {
                std::cerr << "accept error" << std::endl;
            }
            else
            {
                fcntl(new_socket_client, F_SETFL, O_NONBLOCK);

                for (int i = 0; i < max_clients; i++) // add new_socket_client au tableau de socket
                {
                    if (client_socket[i] == 0)
                    {
                        client_socket[i] = new_socket_client;
                        break;
                    }
                }
            }
            //else its some IO operation on some other socket
            for (int i = 0; i < max_clients; i++)
            {
                max_sd = client_socket[i];

                if (FD_ISSET(max_sd, &readfds))
                {
                    //Check if it was for closing , and also read the 
                    //incoming message 
                    if ((valread = read(max_sd, buffer, 1024)) == 0)
                    {
                        //Somebody disconnected , get his details and print 
                        getpeername(max_sd, (struct sockaddr *)&_addr, (socklen_t *) &addrlen);
                        std::cout << "ip = " << inet_ntoa(_addr.sin_addr) << "port = " << ntohs(_addr.sin_port) << std::endl;

                        //Close the socket and mark as 0 in list for reuse
                        close(max_sd);  
                        client_socket[i] = 0;
                    }
                    //echo back the message that came in
                    else
                    {
                        //set the string terminating NULL byte on the end 
                        //of the data read
                        buffer[valread] = '\0'; 
                        send(max_sd, buffer, strlen(buffer), 0);
                    }
                }
            }
         }
    }
}
