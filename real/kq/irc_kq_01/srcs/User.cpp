#include "../includes/User.hpp"
#include "../includes/Server.hpp"

User::User() : Command(5){}

void	User::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	(void)channels;
	(void)changelist;
	
	if (!checkArgs(cmdlist, client))
		return ;
	else if (client.getUserInfo().size() == 4)
	{
		makeWriteEvent(client.getUserSock(), changelist, ERR_ALREADYREGISTRED(client.getNickname(), cmdlist[0]));
		return ;
	}
	else
	{
		for (int i = 1; i < 5; i++)
			client.setUserInfo(cmdlist[i]);
		if (client.getNickname() != "*")
		{
			client.setIsRegistered();
			makeWriteEvent(client.getUserSock(), changelist, RPL_WELCOME(client.getNickname(), client.getUserName(), client.getHostName()));
		}
	}
}
bool	User::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (static_cast<int>(cmdlist.size()) < getRequiredArgsNumber())
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
		return (false);
	}
	return (true);
}
