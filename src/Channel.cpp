#include "../includes/Channel.hpp"

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

}

void	Channel::removeClient(int fd)
{

}

void	Channel::broadcastMessage(const std::string &message)
{

}
