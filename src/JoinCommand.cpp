#include "../includes/JoinCommand.hpp"

void JoinCommand::execute(const std::vector<std::string>& args, int socket, Server& server) {
    // Perform the join logic here
    // For example, add the user to a channel in globalState
    std::cout << "Join command executed" << std::endl;
    (void) args;
    (void) socket;
    (void) server;
}
