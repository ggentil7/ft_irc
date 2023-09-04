#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

Client::Client() : _nickname("default_nick"), _username("default_user"), _hostname("default_host"), _realname("default_real"), _isOperator(false)
{
	(void) _isOperator; //!
}

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

int	Client::getFd(void) const
{
	return _fd;
}

std::string	Client::getNickname(void) const
{
    return _nickname;
}

std::string	Client::getUsername(void) const
{
    return _username;
}

std::string	Client::getHostname(void) const
{
    return _hostname;
}

std::string	Client::getRealname(void) const
{
    return _realname;
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void	Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void	Client::setRealname(std::string realname)
{
	this->_realname = realname;
}

void Client::addInvite(const std::string &channelName)
{
	_invitations.push_back(channelName);
}

bool Client::hasInvite(const std::string &channelName)
{
	return std::find(_invitations.begin(), _invitations.end(), channelName) != _invitations.end();
}

void Client::removeInvite(const std::string &channelName)
{
	 std::vector<std::string>::iterator it = std::find(_invitations.begin(), _invitations.end(), channelName);
        if (it != _invitations.end()) 
		{
            _invitations.erase(it);
        }
}
