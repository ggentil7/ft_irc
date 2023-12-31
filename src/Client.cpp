#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

Client::Client() : _nickname("default_nick"), _username("default_user"), _hostname("default_host"), _realname("default_real"), _registration(Client::FAIL), _clientModes(Client::NONE) {}

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

int Client::getFd(void) const
{
	return _fd;
}

std::string Client::getNickname(void) const
{
	return _nickname;
}

std::string Client::getUsername(void) const
{
	return _username;
}

std::string Client::getHostname(void) const
{
	return _hostname;
}

std::string Client::getRealname(void) const
{
	return _realname;
}

int	Client::getRegistration(void) const
{
	return _registration;
}

bool	Client::getQuit(void) const
{
	return _quit;
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}

void Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void Client::setUsername(std::string username)
{
	this->_username = username;
}

void Client::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void Client::setRealname(std::string realname)
{
	this->_realname = realname;
}


void	Client::setRegistration(int registration)
{
	this->_registration = registration;
}

void	Client::setQuit(bool quit)
{
	this->_quit = quit;
}

void Client::setMode(int modeFlag, bool enable)
{
	if (enable)
		_clientModes |= modeFlag;  // Enable the mode by setting the bit
	else
		_clientModes &= ~modeFlag;  // Disable the mode by clearing the bit
}

bool	Client::isModeSet(int modeFlag) const
{
	return (_clientModes & modeFlag) != 0;
}
