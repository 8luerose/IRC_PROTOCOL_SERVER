#include "../includes/Quit.hpp"
#include "../includes/Server.hpp"

Quit::Quit() : Command(1){};

void  Quit::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
  Server &server = Server::getInstance();
  (void) channels;
  client.setIsQuit();
  // 내가접속한 채널들 돌면서 나를 찾아서 제거
  for (std::vector<Channel *>::iterator joinedChannelIt = client.getJoinedChannel().begin(); joinedChannelIt != client.getJoinedChannel().end(); ++joinedChannelIt ){
    if ((*joinedChannelIt)->getClients().size() == 0)
      continue;
    for (std::vector<Client *>::iterator in = (*joinedChannelIt)->getClients().begin(); in != (*joinedChannelIt)->getClients().end(); ++in){
      if ((*in)->getNickname() == client.getNickname()){
        (*joinedChannelIt)->getClients().erase(in);
        if ((*joinedChannelIt)->getClients().size() > 0)
          (*joinedChannelIt)->setOpFd((*joinedChannelIt)->getClients().front()->getUserSock());
        break;
      }
    } 
  }
  // 내가 접속한 채널 돌면서 quit 메세지 전송
  for (std::vector<Channel *>::iterator joinedChannelIt = client.getJoinedChannel().begin(); joinedChannelIt != client.getJoinedChannel().end(); ++joinedChannelIt ){
    for (std::vector<Client *>::iterator in = (*joinedChannelIt)->getClients().begin(); in != (*joinedChannelIt)->getClients().end(); ++in) // TODO: 어떤 조건일 때 터지는지 조사가 필요함
      makeWriteEvent((*in)->getUserSock(), changelist, SUCCESS_REPL(client.getNickname(), mergeVec(cmdlist)));
  }
  
  // 내가 제거된 채널에 더이상 클라이언트가 없으면 서버에서 제거
  for (std::vector<Channel *>::iterator joinedChannelIt = client.getJoinedChannel().begin(); joinedChannelIt != client.getJoinedChannel().end(); ++joinedChannelIt ){
    if ((*joinedChannelIt)->getClients().size() == 0)
      server.getChannels().erase((*joinedChannelIt)->getChannelName());
  }
}

bool  Quit::checkArgs(std::vector<std::string>& cmdlist, Client& client){
  (void) cmdlist;
  (void) client;
  return true;
};
