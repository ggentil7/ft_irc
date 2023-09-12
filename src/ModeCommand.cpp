#include "../includes/ModeCommand.hpp"

int	ModeCommand::parseChannelMode(const std::string &modeStr)
{
	int		modeFlag = 0;
	bool	adding = true;

	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		char c = modeStr[i];
		if (c == '+')
		{
			adding = true;
			continue;
		}
		else if (c == '-')
		{
			adding = false;
			continue;
		}

		if (adding)
		{
			if (c == 'i')
				modeFlag |= Channel::INVITE_ONLY;
			else if (c == 't')
				modeFlag |= Channel::TOPIC_PROTECTED;
			else if (c == 'k')
				modeFlag |= Channel::KEY_PROTECTED;
			else if (c == 'o')
				modeFlag |= Channel::OPERATOR;
			else if (c == 'l')
				modeFlag |= Channel::USER_LIMIT;
		}
		else
		{
			if (c == 'i')
				modeFlag &= Channel::INVITE_ONLY;
			else if (c == 't')
				modeFlag &= Channel::TOPIC_PROTECTED;
			else if (c == 'k')
				modeFlag &= Channel::KEY_PROTECTED;
			else if (c == 'o')
				modeFlag &= Channel::OPERATOR;
			else if (c == 'l')
				modeFlag &= Channel::USER_LIMIT;
		}
	}
	return modeFlag;
}

int	ModeCommand::parseClientMode(const std::string &modeStr)
{
	int modeFlag = 0; // Initialize flag
	bool adding = true; // To keep track if we are adding (+) or removing (-) modes

	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		char c = modeStr[i];
		if (c == '+')
		{
			adding = true;
			continue;
		}
		else if (c == '-')
		{
			adding = false;
			continue;
		}

		if (adding)
		{
			if (c == 'a')
				modeFlag |= Client::AWAY;
			else if (c == 'i')
				modeFlag |= Client::INVISIBLE;
			else if (c == 'w')
				modeFlag |= Client::WALLOPS;
			else if (c == 'r')
				modeFlag |= Client::RESTRICTED;
			else if (c == 'o')
				modeFlag |= Client::OPERATOR;
			else if (c == 's')
				modeFlag |= Client::SERVER_NOTICES;
		}
		else
		{
			if (c == 'a')
				modeFlag &= Client::AWAY;
			else if (c == 'i')
				modeFlag &= Client::INVISIBLE;
			else if (c == 'w')
				modeFlag &= Client::WALLOPS;
			else if (c == 'r')
				modeFlag &= Client::RESTRICTED;
			else if (c == 'o')
				modeFlag &= Client::OPERATOR;
			else if (c == 's')
				modeFlag &= Client::SERVER_NOTICES;
		}
	}
	return modeFlag;
}

void ModeCommand::execute(const std::vector<std::string> &args, int client_fd, Server &server)
{
	if (args.size() < 2)
	{
		server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string	target = args[0];
	std::string	mode = args[1];
	std::string	additional_arg;

	if (target[0] == '&' || target[0] == '#' || target[0] == '+' || target[0] == '!')
	{ // This is a channel mode change
		Channel *channel = server.getChannelByName(target);
		if (!channel)
		{
			server.sendReply(":ft_irc 403 " + server.getClients()[client_fd]->getNickname() + " " + target + " :No such channel", client_fd); // ERR_NOSUCHCHANNEL
			return;
		}

		if (!channel->isOperator(client_fd))
		{
			server.sendReply(":ft_irc 482 " + server.getClients()[client_fd]->getNickname() + " " + target + " :You're not channel operator", client_fd); // ERR_CHANOPRIVSNEEDED
			return;
		}

		int modeFlag = parseChannelMode(mode);

		// Check if 'k' mode is being set
		if ((modeFlag & Channel::KEY_PROTECTED) != 0)
		{
			if (args.size() < 3)
			{
				server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd);
				return;
			}
			additional_arg = args[2];
			channel->setKey(additional_arg);
		}

		// Check if 'l' mode is being set
		if ((modeFlag & Channel::USER_LIMIT) != 0)
		{
			if (args.size() < 3) // Check if limit argument is provided
			{
				server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd);
				return;
			}
			size_t userLimit = std::atoi(args[2].c_str());
			channel->setUserLimit(userLimit);
		}

		channel->setMode(modeFlag, true);

		std::string modeMessage = ":" + server.getClients()[client_fd]->getNickname() + " MODE " + target + " :" + mode;
		if (!additional_arg.empty())
			modeMessage += " " + additional_arg;

		// Notify all channel members
		channel->broadcastMessage(modeMessage, server);
	}
	else
	{ // This is a user mode change
		Client *client = server.getClientByNickname(target);
		if (!client)
		{
			server.sendReply(":ft_irc 401 " + server.getClients()[client_fd]->getNickname() + " " + target + " :No such nick/channel", client_fd); // ERR_NOSUCHNICK
			return;
		}

		int modeFlag = parseClientMode(mode);

		// Ignore +o as users could bypass the authentication mechanisms of the OPER command
		if ((modeFlag & Client::OPERATOR) != 0)
			return;

		client->setMode(modeFlag, true);

		server.sendReply(":ft_irc 221 " + client->getNickname() + " :" + mode, client_fd);
	}
}
