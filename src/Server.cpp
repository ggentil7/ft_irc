#incldue "Server.hpp"

// Server::Server() :_port(0), _socket(0);
// {
// }

Server::Server(int port) : _port(port), _socket(-1)
{
}

Server::Server(Server const &src)
{
    *this = src;
}

Server &Server::operator=(Server const &src)
{
    if (this != src)
    {
        _socket = src._socket;
        _port = src._port;
        _addr = src._addr;
    }
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
        std::cerr << "setsockopt failed" std::endl;;
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
    socklen_t addrlen = sizeof(address);
    char buffer[1025];

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
    }
}