#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/Server.hpp"
#include "../includes/CapCommand.hpp"
#include "../includes/InviteCommand.hpp"
#include "../includes/ModeCommand.hpp"
#include "../includes/WhoisCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/PrivmsgCommand.hpp"

# define DEFAULT "\001\033[0;39m\002"
# define GRAY "\001\033[1;90m\002"
# define RED "\001\033[1;91m\002"
# define GREEN "\001\033[1;92m\002"
# define YELLOW "\001\033[1;93m\002"
# define BLUE "\001\033[1;94m\002"
# define MAGENTA "\001\033[1;95m\002"
# define CYAN "\001\033[1;96m\002"
# define WHITE "\001\033[0;97m\002"

Server::Server() {}

Server::Server(int port, std::string password) : _port(port), _password(password),_socket(0), _validPassword(false)
{
	_commandMap["PASS"] = new PassCommand();
	_commandMap["NICK"] = new NickCommand();
	_commandMap["JOIN"] = new JoinCommand();
	_commandMap["CAP"] = new CapCommand();
	_commandMap["INVITE"] = new InviteCommand();
	_commandMap["MODE"] = new ModeCommand();
	_commandMap["PASS"] = new PassCommand();
	_commandMap["WHOIS"] = new WhoisCommand();
	_commandMap["PRIVMSG"] = new PrivmsgCommand();
}

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
	for (std::map<std::string, ICommand *>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it)
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

std::map<int, Client *> Server::getClients()
{
	return (this->_clients);
}

std::map<std::string, Channel*> Server::getChannels()
{
	return (this->_channels);
}

std::map<std::string, ICommand *> Server::getCommands()
{
	return (this->_commandMap);
}

std::string Server::getPassword()
{
	return (_password);
}

void	Server::setPassword(std::string password)
{
	_password = password;
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

			std::string	defaultNickBase = "guest";
			std::string	defaultNick;
			int	uniqueID = 0;

			do
				defaultNick = defaultNickBase + std::to_string(uniqueID++);
			while (isNickInUse(defaultNick));

			_clients[new_socket_client] = new Client();
			_clients[new_socket_client]->setNickname(defaultNick);

			std::string nick = this->_clients[_client_socket.back()]->getNickname();
		}

		for (size_t i = 0; i < _client_socket.size(); i++)
		{
			int sd = _client_socket[i];
			if (FD_ISSET(sd, &readfds))
			{
				//TODO handle CTRL-D
				std::map<int, std::string> clientBuffers; // Store buffer for each client

				int valread;
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					// Handle disconnection logic
					close(sd);
					_client_socket.erase(_client_socket.begin() + i);
					clientBuffers.erase(sd); // Remove the buffer for the disconnected client
				}
				else
				{
					buffer[valread] = '\0';
					clientBuffers[sd] += std::string(buffer); // Append new data to the existing buffer

					size_t pos = 0;
					std::string &incomingBuffer = clientBuffers[sd]; // Reference to the buffer for easier access

					// Loop to find each command separated by \r\n
					while ((pos = incomingBuffer.find("\r\n")) != std::string::npos)
					{
						std::string singleCommand = incomingBuffer.substr(0, pos);

						if (singleCommand.substr(0, 4) != "PING" && singleCommand.substr(0, 4) != "PONG")
						{
							std::cout << RED << "Client: [" << sd << "->" << this->_socket << "] " << singleCommand << std::endl;
						}

						std::pair<std::string, std::vector<std::string> > parsedData = parse(singleCommand);
						std::string command = parsedData.first;
						std::vector<std::string> args = parsedData.second;
						if (_commandMap.find(command) != _commandMap.end())
						{
							ICommand *commandHandler = _commandMap[command];
							commandHandler->execute(args, _client_socket[i], *this);
						}

						// Remove the processed command from the buffer
						incomingBuffer.erase(0, pos + 2);
					}
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
	// Iterate through the map of clients
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// Check if the nickname is already in use
		if (it->second->getNickname() == nick)
		{
			return true; // Nickname is in use
		}
	}
	return (false);
}

void Server::sendReply(const std::string &message, int client_fd)
{
	// Send the IRC reply back to the client
	std::string formattedMessage = message + "\r\n";
	const char *cMessage = formattedMessage.c_str();
	std::cout << GREEN <<"Server: [" << this->_socket << "->" << client_fd << "(" << _clients[client_fd]->getNickname() << ")" << "] " << cMessage;
	if (send(client_fd, cMessage, std::strlen(cMessage), 0) == -1)
	{
		// Log the error or handle it appropriately
		std::cerr << "Failed to send reply to client: " << client_fd << std::endl;
	}
}

bool Server::sendMessage(const std::string &recipient, const std::string &message, Client *sender)
{
	// Format the message according to IRC standards
	std::string formattedMessage = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " PRIVMSG " + recipient + " :" + message + "\r\n";

	// Check if the recipient is a channel
	if (_channels.find(recipient) != _channels.end())
	{
		std::list<Client *> members = _channels[recipient]->getMembers();
		for (std::list<Client *>::iterator it = members.begin(); it != members.end(); ++it)
		{
			// Do not send the message back to the sender
			if ((*it)->getFd() != sender->getFd())
			{
				std::cout << "Server: [" << sender->getFd() << "(" << sender->getNickname()  << ")" << "->" << (*it)->getFd() << "(" << (*it)->getNickname() << ")" << "] " << formattedMessage;
				if (send((*it)->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
					std::cerr << "Failed to send message to client: " << (*it)->getFd() << std::endl;
			}
		}
		return true;
	}
	// Check if the recipient is a client
	else
	{
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->second->getNickname() == recipient || it->second->getUsername() == recipient)
			{
				std::cout << "Sending to : " << _clients[it->first]->getNickname() << std::endl;  // Debugging output
				std::cout << "Formatted Message: " << formattedMessage << std::endl;
				for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
				{
					std::cout << "Checking client with FD: " << it->first << " and Nickname: " << it->second->getNickname() << std::endl;
					// ... rest of your code
				}
				std::cout << "send is to " << it->first << std::endl;
				if (send(it->first, formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
					std::cerr << "Failed to send message to client: " << it->first << std::endl;
				return true;
			}
		}
	}
	// The recipient does not exist
	return false;
}
