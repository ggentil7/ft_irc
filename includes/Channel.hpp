#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>

class Channel
{
private:

public:
	Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &rhs);
	~Channel();

};

#endif
