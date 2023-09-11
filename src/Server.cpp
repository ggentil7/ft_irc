#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/Server.hpp"
#include "../includes/CapCommand.hpp"
#include "../includes/InviteCommand.hpp"
#include "../includes/ModeCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/PrivmsgCommand.hpp"
#include "../includes/UserCommand.hpp"
#include "../includes/KickCommand.hpp"
#include "../includes/TopicCommand.hpp"

Server::Server() {}

Server::Server(int port, std::string password) : _port(port), _password(password),_socket(0), _validPassword(false)
{
	_commandMap["CAP"] = new CapCommand();
	_commandMap["PASS"] = new PassCommand();
	_commandMap["NICK"] = new NickCommand();
	_commandMap["USER"] = new UserCommand();
	_commandMap["JOIN"] = new JoinCommand();
	_commandMap["KICK"] = new KickCommand();
	_commandMap["INVITE"] = new InviteCommand();
	_commandMap["TOPIC"] = new TopicCommand();
	_commandMap["MODE"] = new ModeCommand();
	_commandMap["PRIVMSG"] = new PrivmsgCommand();
	_commandMap["OPER"] = new OperCommand();
	_commandMap["PING"] = new PingCommand();
	_commandMap["QUIT"] = new QuitCommand();
	_commandMap["NOTICE"] = new NoticeCommand();
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

Channel	*Server::getChannelByName(const std::string name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(name);
	if (it != _channels.end())
	{
		return it->second;
	}
	return NULL;
}

Client	*Server::getClientByNickname(const std::string nickname)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
		{
			return it->second;
		}
	}
	return NULL;
}

Client	*Server::getClientByFd(int fd)
{
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return it->second;
	return NULL;
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

			_clients[new_socket_client] = new Client();
			_clients[new_socket_client]->setFd(new_socket_client);
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
					removeClient(sd);
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

						std::cout << RED << "Client: [" << sd << "->" << this->_socket << "] " << singleCommand  << DEFAULT << std::endl;

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
	std::vector<std::string> args;

	// Extract prefix if exists
	iss >> token;
	if (token[0] == ':')
	{
		prefix = token.substr(1);
		iss >> command;
	}
	else
		command = token;

	// Extract arguments
	while (iss >> token)
	{
		if (token[0] == ':')
		{
			std::string remaining;
			getline(iss, remaining);
			args.push_back(token.substr(1) + remaining);
			break;
		}
		else
			args.push_back(token);
	}
	return std::make_pair(command, args);
}

bool Server::isNickInUse(const std::string &nick)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return true;
	}
	return (false);
}

void Server::sendReply(const std::string &message, int client_fd)
{
	std::string formattedMessage = message + "\r\n";
	const char *cMessage = formattedMessage.c_str();

	std::cout << GREEN <<"Server: [" << this->_socket << "->" << client_fd << "(" << _clients[client_fd]->getNickname() << ")" << "] " << cMessage << DEFAULT;

	if (send(client_fd, cMessage, std::strlen(cMessage), 0) == -1)
		std::cerr << "Failed to send reply to client: " << client_fd << std::endl;
}

bool Server::sendMessage(const std::string &recipient, const std::string &message, Client *sender)
{
	// Format the message according to IRC standards
	std::string formattedMessage = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " PRIVMSG " + recipient + " :" + message + "\r\n";

	// Check if the recipient is a channel
	if (_channels.find(recipient) != _channels.end())
	{
		std::list<int> members = _channels[recipient]->getMembers();
		for (std::list<int>::iterator it = members.begin(); it != members.end(); ++it)
		{
			int recipientFd = *it;
			Client	*recipientClient = getClientByFd(recipientFd);
			// Do not send the message back to the sender
			if (recipientFd != sender->getFd())
			{
				std::cout << GREEN << "Server: [" << sender->getFd() << "(" << sender->getNickname()  << ")" << "->" << recipientFd << "(" << recipientClient->getNickname() << ")" << "] " << formattedMessage << DEFAULT;
				if (send(recipientFd, formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
					std::cerr << "Failed to send message to client: " << recipientFd << std::endl;
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
				int recipientFd = it->first;
				Client	*recipientClient = getClientByFd(recipientFd);

				std::cout << GREEN << "Server: [" << sender->getFd() << "(" << sender->getNickname()  << ")" << "->" << recipientFd << "(" << recipientClient->getNickname() << ")" << "] " << formattedMessage << DEFAULT;
				if (send(recipientFd, formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
					std::cerr << "Failed to send message to client: " << recipientFd << std::endl;
				return true;
			}
		}
	}
	// The recipient does not exist
	return false;
}

void Server::addChannel(const std::string& channelName, Channel* channel)
{
	_channels[channelName] = channel;
}

void	Server::removeClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it != _clients.end())
	{
		// Delete the client object from heap
		delete it->second;

		// Remove the client from the map
		_clients.erase(it);
	}

	// Close the socket
	close(client_fd);
}
