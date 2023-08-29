#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

Command::Command(int client_fd, const std::string &message, std::map<int, Client> &clients)
	: _client_fd(client_fd), _message(message), _clients(clients)
{
	initCommandMap();
}

void Command::initCommandMap()
{
	_commandMap["NICK"] = &Command::handleNick;
	_commandMap["JOIN"] = &Command::handleJoin;
	_commandMap["PRIVMSG"] = &Command::handlePrivMsg;
}

void Command::parse()
{
	std::istringstream iss(_message);
	std::string token;

	// Check for prefix
	std::string _prefix;
	iss >> token;
	if (token[0] == ':')
	{
		_prefix = token.substr(1); // Remove the leading ':'
		iss >> _command;		   // The next token should be the command
	}
	else
	{
		_command = token; // No prefix, the first token is the command
	}

	// Parse parameters
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
			_params.push_back(token.substr(1) + lastParam);
			break;
		}
		else
		{
			_params.push_back(token);
		}
	}
}

void Command::dispatch()
{
	std::map<std::string, void (Command::*)()>::iterator it = _commandMap.find(_command);
	if (it != _commandMap.end())
	{
		(this->*(it->second))();
	}
	else
	{
		// Handle unknown command or send an error reply
	}
}

void Command::handleNick()
{
	if (_params.size() < 1) //! Send error reply: ERR_NONICKNAMEGIVEN (431)
        return;
	std::string newNick = _params[0];
	if (_clients.find(_client_fd) != _clients.end())
		_clients[_client_fd].setNickname(newNick);
	else
		return; //! error reply back client not registered
}

void Command::handleJoin()
{
   if (_params.size() < 1)  // Send error reply: ERR_NEEDMOREPARAMS (461)
        return;
	std::string	channelName = _params[0];
	//TODO
}

void Command::handlePrivMsg()
{

}
