#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/ICommand.hpp"
#include "../includes/JoinCommand.hpp"
#include "../includes/NickCommand.hpp"
#include "../includes/Server.hpp"

Channel::Channel() {}

Channel::Channel(Channel const &src)
{
	*this = src;
}
Channel &Channel::operator=(Channel const &rhs)
{
	(void)rhs;
	return (*this);
}

Channel::~Channel() {}

// void	Channel::addClient(int fd, Server &server)
// {
// 	Client* clientToAdd = server.getClientByFd(fd);
//     if (clientToAdd) {
//         // Vérifie si le client n'est pas déjà membre du canal
//         if (std::find(_members.begin(), _members.end(), clientToAdd) == _members.end()) 
// 		{
//             _members.push_back(clientToAdd);
            
//             // envoie notif dans channel que qlqun a join 
//             broadcastMessage(clientToAdd->getNickname() + " has joined " + _name, server);
//         }
//     }
// }

void	Channel::addClient(int fd)
{
     _members.push_back(fd);
}

void	Channel::removeClient(int fd)
{
	(void) fd;
}

void	Channel::broadcastMessage(const std::string &message, Server &server)
{
	for (std::list<int>::iterator it = _members.begin(); it != _members.end(); ++it) 
	{
        int clientFd = *it; 

        server.sendReply(message, clientFd);
    }
}

std::list<int>	Channel::getMembers() const
{
	return (_members);
}

void Channel::addInvitedUsers(Client *client)
{
	if (std::find(_invitedUsers.begin(), _invitedUsers.end(), client) == _invitedUsers.end()) 
	{
         _invitedUsers.push_back(client);
    }
}

bool Channel::isUserInvited(Client *client)
{
	return std::find(_invitedUsers.begin(), _invitedUsers.end(), client) != _invitedUsers.end();
}
