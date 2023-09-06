#include "../includes/CapCommand.hpp"
#include <vector>

void CapCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 1)
		return;
	if (args[0] == "LS")
		server.sendReply("CAP * LS :multi-prefix sasl", client_fd);
	else if (args[0] == "REQ" && args[1] == "multi-prefix")
	{
		std::string capAckMsg = "CAP * ACK :multi-prefix";
		server.sendReply(capAckMsg, client_fd);
	}
}
