#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>

#include "Channel.hpp"
#include "Client.hpp"
#include "ICommand.hpp"
#include "Server.hpp"

class Client
{
private:
	int			_fd;
	std::string	_nickname;
	std::string	_username;
	std::string	_hostname;
	std::string	_realname;
	bool		_isOperator;
	std::vector<std::string> _invitations;
public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

	int			getFd(void) const;
	std::string	getNickname(void) const;
	std::string	getUsername(void) const;
	std::string	getHostname(void) const;
	std::string	getRealname(void) const;

	void	setFd(int fd);
	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setHostname(std::string hostname);
	void	setRealname(std::string realname);

	void addInvite(const std::string &channelName);
	void removeInvite(const std::string& channelName);
	bool hasInvite(const std::string &channelName);
};

#endif
