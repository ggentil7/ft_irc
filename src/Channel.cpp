#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

Channel::Channel() : _key("key"), _channelModes(NONE), _userLimit(0) {}

Channel::Channel(Channel const &src)
{
	*this = src;
}
Channel &Channel::operator=(Channel const &rhs)
{
	(void)rhs;
	return (*this);
}

Channel::~Channel() {}

void Channel::broadcastMessage(const std::string &message, Server &server)
{
	for (std::list<int>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		int clientFd = *it;

		server.sendReply(message, clientFd);
	}
}

void Channel::addMember(int client_fd)
{
	_members.push_back(client_fd);
}

void Channel::addOperator(int client_fd)
{
	_operators.push_back(client_fd);
}

bool Channel::isMember(int client_fd)
{
	return std::find(_members.begin(), _members.end(), client_fd) != _members.end();
}

bool Channel::isOperator(int client_fd)
{
	return std::find(_operators.begin(), _operators.end(), client_fd) != _operators.end();
}

void Channel::removeMember(int client_fd)
{
	_members.remove(client_fd);
}

void Channel::removeOperator(int client_fd)
{
	_operators.remove(client_fd);
}

std::list<int> Channel::getMembers() const
{
	return _members;
}

std::list<int> Channel::getOperators() const
{
	return _operators;
}

void Channel::setMode(int modeFlag, bool enable)
{
	if (enable)
		_channelModes |= modeFlag; // Enable the mode by setting the bit
	else
		_channelModes &= ~modeFlag; // Disable the mode by clearing the bit
}

bool Channel::isModeSet(int modeFlag) const
{
	return (_channelModes & modeFlag) != 0;
}

size_t	Channel::getUserLimit() const
{
	return (_userLimit);
}

void	Channel::setUserLimit(size_t limit)
{
	_userLimit = limit;
}

std::string		Channel::getKey() const
{
	return (_key);
}

void	Channel::setKey(std::string key)
{
	_key = key;
}

void Channel::addInvitedUser(Client *client)
{
	if (std::find(_invitedUsers.begin(), _invitedUsers.end(), client) == _invitedUsers.end())
	{
		_invitedUsers.push_back(client);
	}
}

bool Channel::isUserInvited(Client *client)
{
	return std::find(_invitedUsers.begin(), _invitedUsers.end(), client) != _invitedUsers.end();
}

std::string		Channel::getTopic() const
{
	return (_topic);
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string		Channel::getName() const
{
	return (_name);
}

void	Channel::setName(std::string name)
{
	_name = name;
}
