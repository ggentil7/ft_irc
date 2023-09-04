#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

Channel::Channel() {}

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

void	Channel::addClient(int fd)
{
	(void) fd;
}

void	Channel::removeClient(int fd)
{
	(void) fd;
}

void	Channel::broadcastMessage(const std::string &message)
{
	(void) message;
}

std::list<Client*>	Channel::getMembers() const
{
	return (_members);
}
