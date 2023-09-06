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
	bool		_isRegistered;
	int			_clientModes;
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
	bool		getRegistration(void) const;

	void	setFd(int fd);
	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setHostname(std::string hostname);
	void	setRealname(std::string realname);
	void	setRegistration(bool registered);

	void	setMode(int modeFlag, bool enable);
	bool	isModeSet(int modeFlag) const;

	enum Mode {
		NONE			= 0,
		AWAY			= 1 << 0,
		INVISIBLE		= 1 << 1,
		WALLOPS			= 1 << 2,
		RESTRICTED		= 1 << 3,
		OPERATOR		= 1 << 4,
		SERVER_NOTICES	= 1 << 5
	};
};

#endif
