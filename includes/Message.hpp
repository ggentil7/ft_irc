#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include "Client.hpp"

class Message
{
private:
	Client*		sender;
	std::string	content;
	std::string	type;

public:
	Message();
	Message(Message const &src);
	Message &operator=(Message const &rhs);
	~Message();
};

#endif
