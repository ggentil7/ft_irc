#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <vector>
#include <algorithm>

#include "Client.hpp"
#include "ICommand.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel
{
private:
	std::string		_name;
	std::string		_topic;
	std::string		_key;
	std::list<int>	_members;
	std::list<int>	_operators;
	int				_channelModes;
	size_t			_userLimit;
	std::vector<Client *>	_invitedUsers;

public:
	Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &rhs);
	~Channel();

	void	broadcastMessage(const std::string &message, Server &server);

	void	addMember(int client_fd);
	void	addOperator(int client_fd);

	void	removeMember(int client_fd);
	void	removeOperator(int client_fd);

	bool	isMember(int client_fd);
	bool	isOperator(int client_fd);

	std::list<int>	getMembers() const;
	std::list<int>	getOperators() const;

	bool	isModeSet(int modeFlag) const;
	void	setMode(int modeFlag, bool enable);

	size_t	getUserLimit() const;
	void	setUserLimit(size_t limit);

	std::string		getKey() const;
	void			setKey(std::string key);

	void	addInvitedUser(Client *client);
	bool	isUserInvited(Client *client);

	std::string		getTopic() const;
	void			setTopic(std::string topic);

	std::string		getName() const;
	void			setName(std::string name);
	
	enum Mode {
		NONE = 0x0,
		INVITE_ONLY = 0x1,		// i
		TOPIC_PROTECTED = 0x2,	// t
		KEY_PROTECTED = 0x4,	// k
		OPERATOR = 0x8,			// o
		USER_LIMIT = 0x10		// l
	};
};

#endif
