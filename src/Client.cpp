#include "../includes/Command.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

Client::Client() {}

Client::Client(Client const &src)
{
	*this = src;
}
Client &Client::operator=(Client const &rhs)
{
	(void)rhs;
	return (*this);
}

Client::~Client() {}

std::string	Client::getNickname(void) const
{
    return nickname;
}
