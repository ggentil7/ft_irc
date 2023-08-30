#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/Server.hpp"

Server::Server() : _port(0), _socket(0), _validPassword(false)
{
	_commandMap["PASS"] = new PassCommand();
	_commandMap["NICK"] = new NickCommand();
	_commandMap["JOIN"] = new JoinCommand();
	_commandMap["INVITE"] = new InvitCommand();
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
	for (std::map<std::string, ICommand*>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it)
		delete it->second;
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

std::map<int, Client*>	Server::getClients()
{
	return (this->_clients);
}

std::map<std::string, Channel*> Server::getChannel()
{
	return (this->_channels)
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
		exit(EXIT_FAILURE);
	}

	if (listen(_socket, BACK_LOG) < 0)
	{
		std::cerr << "listen failed" << std::endl;
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

		for (size_t i = 0; i < _client_socket.size(); i++)
		{
			int sd = _client_socket[i];
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
			_client_socket.push_back(new_socket_client);

			// Here, you can send the welcome messages to the new client
			_clients[new_socket_client] = new Client();
			_clients[new_socket_client]->setNickname("default_nick");
			std::string welcomeMsg = ":YourServer 001 " + this->_clients[_client_socket.back()]->getNickname() + " :Welcome to the IRC Network " + this->_clients[new_socket_client]->getNickname() + "\r\n";
			send(_client_socket.back(), welcomeMsg.c_str(), welcomeMsg.length(), 0);
		}

		for (size_t i = 0; i < _client_socket.size(); i++)
		{
			int sd = _client_socket[i];
			if (FD_ISSET(sd, &readfds))
			{
				int valread;
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					close(sd);
					_client_socket.erase(_client_socket.begin() + i);
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
					if (_commandMap.find(command) != _commandMap.end())
					{
						ICommand *commandHandler = _commandMap[command];
						commandHandler->execute(args, _client_socket[i], *this);
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
	std::vector<std::string> params;
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

bool Server::isNickInUse(const std::string &nick)
{
	// Check if the nickname is already in use
	// Return true if it is, false otherwise
	(void) nick;
	return (false);
}

void Server::setNick(const std::string &nick, int client_fd)
{
	// Set the new nickname
	(void) nick;
	(void) client_fd;
}

void Server::sendReply(const std::string &message, int client_fd)
{
	// Send the IRC reply back to the client
	std::string formattedMessage = message + "\r\n";
	const char *cMessage = formattedMessage.c_str();
	if (send(client_fd, cMessage, std::strlen(cMessage), 0) == -1)
	{
		// Log the error or handle it appropriately
		std::cerr << "Failed to send reply to client: " << client_fd << std::endl;
	}
}

/*
// cleanup when a client disconnects
int client_fd = // the client's file descriptor
if (this->_clients.find(client_fd) != this->_clients.end()) {
    delete this->_clients[client_fd];
    this->_clients.erase(client_fd);
}
*/
