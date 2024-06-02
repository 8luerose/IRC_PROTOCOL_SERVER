#include "../includes/Pass.hpp"
#include "../includes/Server.hpp"
#include "../includes/Define.hpp"

Pass::Pass() : Command(2){}

void  Pass::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	(void)channels;
	if (checkArgs(cmdlist, client) == false)
		return ;
	if (client.getIsPassed() == true)
	{
		//is already passed
		makeWriteEvent(client.getUserSock(), changelist, ERR_ALREADYREGISTRED(client.getNickname(), cmdlist[0]));
		return ;
	}
	if (cmdlist[1] != Server::getInstance().getPswd())
	{
		client.setIsQuit();
		return ;
	}
	client.setIsPassed();
}

bool  Pass::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
  if (cmdlist.size() < static_cast<unsigned long>(getRequiredArgsNumber()))
  {
    makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
    return (false);
  }
  return (true);
}
