#include "Server.hpp"

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
    
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
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
    char buffer[1025];
    socklen_t addrlen = sizeof(_addr);

    while (true) 
    {
        fd_set readfds;
        bzero(&readfds, sizeof(readfds));

        FD_SET(_socket, &readfds);
        int max_sd = _socket;

        for (size_t i = 0; i < client_socket.size(); i++) 
        {
            int sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) 
        {
            std::cerr << "select() error" << std::endl;
        }

        if (FD_ISSET(_socket, &readfds)) 
        {
            int new_socket_client;
            if ((new_socket_client = accept(_socket, (struct sockaddr*)&_addr, &addrlen)) < 0) 
            {
                std::cerr << "accept error" << std::endl;
                exit(EXIT_FAILURE);
            }
            fcntl(new_socket_client, F_SETFL, O_NONBLOCK);

            // Ajout du nouveau client dans le vector
            client_socket.push_back(new_socket_client);
        }

        for (size_t i = 0; i < client_socket.size(); i++) 
        {
            int sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) 
            {
                int valread;
                if ((valread = read(sd, buffer, 1024)) == 0) 
                {
                    getpeername(sd, (struct sockaddr*)&_addr, &addrlen);
                    std::cout << "ip = " << inet_ntoa(_addr.sin_addr) << " port = " << ntohs(_addr.sin_port) << std::endl;

                    close(sd);
                    client_socket.erase(client_socket.begin() + i);
                } else 
                {
                    buffer[valread] = '\0';
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}


/*void Server::connectionServer()
{
    int new_socket_client;
    int max_clients = 30;
    std::vector<int> client_sockets;
    int valread;
    int sd;
    socklen_t addrlen = sizeof(_addr);
    char buffer[1025];
    //std::map<int, std::string> message;

    while (true)
    {
        fd_set readfds;
        //clear the socket set
        FD_ZERO(&readfds);
        //add socket to set
        FD_SET(_socket, &readfds);
        int max_sd = _socket; //représente le maximum socket descriptor

        //add child sockets to set
         for (int i = 0; i < max_clients; i++)
         {
            sd = client_socket[i]; //sd = socket descriptor

            //if valid sd then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);
            //highest fd need it for the select function
            if (sd > max_sd)
                  max_sd = sd;
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
                exit(EXIT_FAILURE);
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
                sd = client_socket[i];

                if (FD_ISSET(sd, &readfds))
                {
                    //Check if it was for closing , and also read the 
                    //incoming message 
                    if ((valread = read(sd, buffer, 1024)) == 0)
                    {
                        //Somebody disconnected , get his details and print 
                        getpeername(sd, (struct sockaddr *)&_addr, (socklen_t *) &addrlen);
                        std::cout << "ip = " << inet_ntoa(_addr.sin_addr) << "port = " << ntohs(_addr.sin_port) << std::endl;

                        //Close the socket and mark as 0 in list for reuse
                        close(sd);  
                        client_socket[i] = 0;
                    }
                    //echo back the message that came in
                    else
                    {
                        //set the string terminating NULL byte on the end 
                        //of the data read
                        buffer[valread] = '\0'; 
                        send(sd, buffer, strlen(buffer), 0);
                    }
                }
            }
         }
    }
}*/


// void Server::connectionServer()
// {
//     fd_set master;
//     fd_set readfds;
//     struct sockaddr_in serv_addr;
//     struct sockaddr_in client_addr;
//     int fdmax;
//     int listener;
//     int newfd;
//     char buf[256];
//     int nbytes;
//     int yes = 1;
//     socklen_t addrlen;
//     int i, j;

//     FD_ZERO(&master);
//     FD_ZERO(&readfds);

//     while (true)
//     {
//         readfds = master;

//         if (select(fdmax + 1, &readfds, NULL, NULL) == -1)
//         {
//             std::cerr << "select error" << std::endl;
//             exit (1);
//         }
//         for (i = 0; i <= fdmax; i++)
//         {
//             if (FD_ISSET(i, &readfds))
//             {
//                 if (i == listener)
//                 {
//                     addrlen = sizeof(client_addr);
//                     if ((newfd = accept(listener, (struct sockaddr *) &client_addr, &addrlen)) == -1)
//                     {
//                         std::cerr << "accept error" << std::endl;
//                         exit (1);
//                     }
//                     else
//                     {
//                         FD_SET(newfd, &master);
//                         if (newfd > fdmax)
//                         {
//                             fdmax = newfd;
//                         }
//                     }
//                 }
//                 else
//                 {
//                     if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
//                     {
//                         if (nbytes == 0)
//                             std::cout << "select server :" << i << std::endl;
//                     }
//                     else
//                         std::cerr << "recv error" << std::endl;
//                 }
//                 close (i);
//                 FD_CLR(i, &master);
//                 else
//                 {
//                     for (j = 0; j <= fdmax; j++)
//                     {
//                         if (FD_ISSET(j, &master))
//                         {
//                             if (j != listener && j != i)
//                             {
//                                 if (send(j, buf, nbytes, 0) == -1)
//                                 {
//                                     std::cerr << "send error" << std::endl;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }