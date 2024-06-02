#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"
#include <set>

Nick::Nick() : Command(2), SPECIAL("[]\\\\`_^{|}"){}

bool  Nick::checkArgsFormat(std::string& newnick)
{
  if (std::isalpha(newnick.data()[0]) == 0 && SPECIAL.find(newnick.data()[0]) == std::string::npos)
    return (false);
  for (int i = 1; i < static_cast<int>(newnick.length()); i++)
  {
    if (newnick.data()[i] != '-' && (std::isdigit(newnick.data()[i]) == 0) \
      && (std::isalpha(newnick.data()[i]) == 0) && SPECIAL.find(newnick.data()[i]) == std::string::npos)
      return (false);
  }
  return (true);
}

bool  Nick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
  if (cmdlist.size() < static_cast<unsigned long>(getRequiredArgsNumber()))
  {
    makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
    return (false);
  }
  if (client.getNickname() == cmdlist[1])
    return (false);
  if (checkArgsFormat(cmdlist[1]) == false)
  {
    makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_ERRONEOUSNICKNAME(client.getNickname(), cmdlist[1]));
    return (false);
  }
  return (true);
}

bool  Nick::isNickUnique(std::map<int, Client>& clientList, const std::string& candidate)
{
  std::map<int, Client>::const_iterator it;
  
  for (it = clientList.begin(); it != clientList.end(); it++)
    if ((*it).second.getNickname() == candidate)
    {
      if (const_cast<Client&>((*it).second).getIsRegistered())
        return (false);
      else
      {
        const_cast<Client&>((*it).second).setNickname("*");
        return (true);
      }
    }
  return (true);
}

void Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
  Server  &server = Server::getInstance();
  (void)channels;

  if (!checkArgs(cmdlist, client))
    return ;
  else if (isNickUnique(server.getClients(), cmdlist[1]) == false)
    makeWriteEvent(client.getUserSock(), changelist, ERR_NICKNAMEINUSE(client.getNickname(), cmdlist[1]));
  else
  {
    std::string prevName = client.getNickname();
    client.setNickname(cmdlist[1]);
    if (client.getIsRegistered() == false)
    {
	    if (client.getUserInfo().size() == 4)
      {
        client.setIsRegistered();
        makeWriteEvent(client.getUserSock(), changelist, RPL_WELCOME(client.getNickname(), client.getUserName(), client.getHostName()));
        return ;
      }
    }
    else if (client.getJoinedChannel().size() == 0){
        makeWriteEvent(client.getUserSock(), changelist, SUCCESS_REPL(prevName, mergeVec(cmdlist)));
	}
	else
	{
      std::set<int> list;
      list.clear();
      std::vector<Channel *>::iterator	it;
      for (it = client.getJoinedChannel().begin(); it != client.getJoinedChannel().end(); it++)
      {
        std::vector<Client*>::iterator	cl_it;
        for (cl_it = (*it)->getClients().begin(); cl_it != (*it)->getClients().end(); cl_it++)
          list.insert((*cl_it)->getUserSock());
      }
      std::set<int>::iterator s_it;
      for (s_it = list.begin(); s_it != list.end(); s_it++)
        makeWriteEvent(*(s_it), changelist, SUCCESS_REPL(prevName, mergeVec(cmdlist)));
	  }
  }
}


