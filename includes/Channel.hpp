#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <vector>

#include "Client.hpp"
#include "ICommand.hpp"
#include "Server.hpp"

class Client;
class Channel
{
private:
	std::string			_name;
	std::string			_topic;
	std::list<Client*>	_members;
	unsigned int 		_mode;

	enum ModeFlags
	{
		MODE_NONE = 0,		   // No special mode
		MODE_INVITE = 1 << 0,  // Invite-only channel
		MODE_TOPIC = 1 << 1,   // Only operators can change topic
		MODE_KEY = 1 << 2,	   // Channel is password protected
		MODE_OPERATOR = 1 << 3 // Client is an operator
	};

public:
	Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &rhs);
	~Channel();

	void	addClient(int fd);
	void	removeClient(int fd);
	void	broadcastMessage(const std::string &message);

	std::list<Client*>	getMembers() const;

	// Check if a specific mode is set
	bool has_mode(ModeFlags flag) const
	{
		return (_mode & flag) != 0;
	}

	// Enable a specific mode
	void enable_mode(ModeFlags flag)
	{
		_mode |= flag;
	}

	// Disable a specific mode
	void disable_mode(ModeFlags flag)
	{
		_mode &= ~flag;
	}

	// Toggle a specific mode (if it's on, turn it off; if it's off, turn it on)
	void toggle_mode(ModeFlags flag)
	{
		_mode ^= flag;
	}
};

#endif
