#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/Server.hpp"

Server::Server() : _port(0), _socket(0), _validPassword(false)
{
	commandMap["JOIN"] = new JoinCommand();
	commandMap["NICK"] = new NickCommand();
}

// Server::Server(int port, std::string password) + validPassowrd a initialiser false
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
	_password = src._password;
	_validPassword = src._validPassword;

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

std::string Server::getPassword()
{
	return (_password);
}

bool Server::getValidPassword()
{
	return (_validPassword);
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
	_addr.sin_addr.s_addr = inet_addr(IP_SERVER);
	_addr.sin_port = htons(_port);

	if (bind(_socket, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		;
		exit(EXIT_FAILURE);
	}

	if (listen(_socket, BACK_LOG) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		;
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
			if ((new_socket_client = accept(_socket, (struct sockaddr *)&_addr, &addrlen)) < 0)
			{
				std::cerr << "accept error" << std::endl;
				exit(EXIT_FAILURE);
			}
			fcntl(new_socket_client, F_SETFL, O_NONBLOCK);

			// Ajout du nouveau client dans le vector
			client_socket.push_back(new_socket_client);

			// Here, you can send the welcome messages to the new client
			std::string clientNick = "some_default_nick"; // You'll probably have a way to get the client's chosen nick
			std::string welcomeMsg = ":YourServer 001 " + clientNick + " :Welcome to the IRC Network " + clientNick + "\r\n";
			send(new_socket_client, welcomeMsg.c_str(), welcomeMsg.length(), 0);
		}

		for (size_t i = 0; i < client_socket.size(); i++)
		{
			int sd = client_socket[i];
			if (FD_ISSET(sd, &readfds))
			{
				int valread;
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					getpeername(sd, (struct sockaddr *)&_addr, &addrlen); // a supprimer car pour debug
					std::cout << "ip = " << inet_ntoa(_addr.sin_addr) << " port = " << ntohs(_addr.sin_port) << std::endl;

					close(sd);
					client_socket.erase(client_socket.begin() + i);
				}
				else
				{
					buffer[valread] = '\0';
					if (std::string(buffer).find("CAP LS") != std::string::npos)
					{
						std::string capResponse = ":YourServer CAP * LS :\r\n";
						send(sd, capResponse.c_str(), capResponse.length(), 0);
					}
					std::string incomingMessage = std::string(buffer);
					std::pair<std::string, std::vector<std::string> > parsedData = parse(incomingMessage);
					std::string command = parsedData.first;
					std::vector<std::string> args = parsedData.second;
					if (commandMap.find(command) != commandMap.end())
					{
						ICommand *commandHandler = commandMap[command];
						commandHandler->execute(args, client_socket[i], *this);
					}
					send(sd, buffer, strlen(buffer), 0); // pour avoir le echo back
				}
			}
		}
	}
}

std::pair<std::string, std::vector<std::string> > Server::parse(std::string message)
{
	std::istringstream iss(message);
	std::string token, prefix, command;

	// Check for prefix
	iss >> token;
	if (token[0] == ':')
	{
		prefix = token.substr(1); // Remove the leading ':'
		iss >> command;			  // The next token should be the command
	}
	else
	{
		command = token; // No prefix, the first token is the command
	}

	// Parse parameters
	std::vector<std::string>	params;
	while (iss >> token)
	{
		if (token[0] == ':')
		{
			// This is the last parameter, which can contain spaces
			std::string lastParam;
			getline(iss, lastParam); // Read the rest of the line
			if (!lastParam.empty())
			{
				lastParam = lastParam.substr(1); // Remove leading space
			}
			params.push_back(token.substr(1) + lastParam);
			break;
		}
		else
		{
			params.push_back(token);
		}
	}
	return (std::make_pair(command, params));
}
