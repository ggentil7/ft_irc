#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>
#include <map>
#include <vector>

class Server;

class ICommand
{
public:
	virtual ~ICommand() {}

	// Define the function to be implemented by each command class
	virtual void execute(const std::vector<std::string>& args, int socket, Server& server) = 0;
};

#endif // ICOMMAND_HPP
