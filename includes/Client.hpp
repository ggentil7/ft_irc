#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>

#include "Channel.hpp"
#include "Command.hpp"
#include "Server.hpp"

class Client
{
private:
	int	socket;
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	realname;
	std::list<std::string>	channels;
public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

	std::string	getNickname(void) const;
	std::string	getUsername(void) const;
	std::string	getHostname(void) const;
	std::string	getRealname(void) const;

	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setHostname(std::string hostname);
	void	setRealname(std::string realname);

};

#endif
