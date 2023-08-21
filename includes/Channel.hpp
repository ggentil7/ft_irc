#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <vector>
#include "Client.hpp"

class Channel
{
private:
	std::string name;
	std::string topic;
	std::list<Client *> members;
	std::list<Client *> operators;
	unsigned int mode;

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

	// Check if a specific mode is set
	bool has_mode(ModeFlags flag) const
	{
		return (mode & flag) != 0;
	}

	// Enable a specific mode
	void enable_mode(ModeFlags flag)
	{
		mode |= flag;
	}

	// Disable a specific mode
	void disable_mode(ModeFlags flag)
	{
		mode &= ~flag;
	}

	// Toggle a specific mode (if it's on, turn it off; if it's off, turn it on)
	void toggle_mode(ModeFlags flag)
	{
		mode ^= flag;
	}
};

#endif
