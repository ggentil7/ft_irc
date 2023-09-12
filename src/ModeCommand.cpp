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
	Client	*client = server.getClients()[client_fd];
	if (!client)
		return;

	if (args.size() < 1)
	{
		server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string	targetName = args[0];

	if (args.size() == 1)
	{
		std::string	modeString = "mode_string"; // TODO
		std::string	modeArgs = "mode_arguments"; // TODO
		server.sendReply(":ft_irc 324 " + client->getNickname() + " " + targetName + " " + modeString + " " + modeArgs, client_fd); // RPL_CHANNELMODEIS
		return;
	}

	std::string	mode = args[1];
	std::string	additional_arg;

	if (args.size() >= 3)
		additional_arg = args[2];

	if (targetName[0] == '&' || targetName[0] == '#' || targetName[0] == '+' || targetName[0] == '!')
	{ // This is a channel mode change
		Channel *channel = server.getChannelByName(targetName);
		if (!channel)
		{
			server.sendReply(":ft_irc 403 " + client->getNickname() + " " + targetName + " :No such channel", client_fd); // ERR_NOSUCHCHANNEL
			return;
		}

		if (!channel->isOperator(client_fd))
		{
			server.sendReply(":ft_irc 482 " + client->getNickname() + " " + targetName + " :You're not channel operator", client_fd); // ERR_CHANOPRIVSNEEDED
			return;
		}

		int modeFlag = parseChannelMode(mode);

		// Check if 'k' mode is being set
		if ((modeFlag & Channel::KEY_PROTECTED) != 0)
		{
			if (args.size() < 3)
			{
				server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
				return;
			}
			channel->setKey(additional_arg);
		}

		// Check if 'l' mode is being set
		if ((modeFlag & Channel::USER_LIMIT) != 0)
		{
			if (args.size() < 3) // Check if limit argument is provided
			{
				server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
				return;
			}
			size_t additional_arg = std::atoi(args[2].c_str());
			channel->setUserLimit(additional_arg);
		}

		// Check if 'o' mode is being set
		if ((modeFlag & Channel::OPERATOR) != 0)
		{
			if (args.size() < 3)
			{
				server.sendReply(":ft_irc 461 MODE :Not enough parameters", client_fd); // ERR_NEEDMOREPARAMS
				return;
			}
			Client	*targetClient = server.getClientByNickname(additional_arg);
			if (!targetClient)
			{
				server.sendReply(":ft_irc 401 " + client->getNickname() + " " + additional_arg + " :No such nick/channel", client_fd); // ERR_NOSUCHNICK
				return;
			}
			if (!channel->isMember(targetClient->getFd()))
			{
				server.sendReply(":ft_irc 441 " + client->getNickname() + " " + additional_arg + " " + targetName + " :They aren't on that channel", client_fd); // ERR_USERNOTINCHANNEL
				return;
			}
			channel->addOperator(targetClient->getFd());
		}

		channel->setMode(modeFlag, true);

		std::string modeMessage = ":" + client->getNickname() + " MODE " + targetName + " :" + mode;
		if (!additional_arg.empty())
			modeMessage += " " + additional_arg;

		// Notify all channel members
		channel->broadcastMessage(modeMessage, server);
	}
	else
	{ // This is a user mode change
		Client *client = server.getClientByNickname(targetName);
		if (!client)
		{
			server.sendReply(":ft_irc 401 " + client->getNickname() + " " + targetName + " :No such nick/channel", client_fd); // ERR_NOSUCHNICK
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
