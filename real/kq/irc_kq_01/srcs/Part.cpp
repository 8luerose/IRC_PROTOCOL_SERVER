#include "../includes/Part.hpp"
#include "../includes/Define.hpp"
#include "../includes/Server.hpp"
#include <sstream>

Part::Part() : Command(0){};

void  Part::execute(std::vector<std::string>& cmdlist, Client& client, \
		std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server& server = Server::getInstance();
	
	if (cmdlist.size() < 2){
		makeWriteEvent(client.getUserSock(), changelist, ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
	}
	else {
		std::stringstream ss(cmdlist[1]);
		std::string token;
		while (getline(ss, token, ','))
		{
			token.erase(0, 1);
			std::map<std::string, Channel>::iterator found = channels->find(token);
			if (found == Server::getInstance().getChannels().end()){
				makeWriteEvent(client.getUserSock(), changelist, ERR_NOSUCHCHANNEL(client.getNickname(), token));
				continue;
			}

			Channel &targetChannel = found->second;
			if (targetChannel.findJoinClient(client.getNickname()) == false){
				makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOTONCHANNEL(client.getNickname(), token));
				continue;
			}
			
			if (targetChannel.getClients().size() > 0){
				std::vector<std::string> reply = cmdlist;
				reply[1] = "#" + targetChannel.getChannelName();
				for (std::vector<Client *>::iterator it = targetChannel.getClients().begin(); it != targetChannel.getClients().end(); ++it)
					makeWriteEvent((*it)->getUserSock(), server.getChangeList(), SUCCESS_REPL(client.getNickname(), mergeVec(reply)));
			}
			// 채널의 접속자 리스트에서 나를 찾아서 삭제
			for (std::vector<Client *>::iterator it = targetChannel.getClients().begin(); it != targetChannel.getClients().end(); ++it){
				if ((*it)->getNickname() == client.getNickname()){
					targetChannel.getClients().erase(it);
					if (targetChannel.getClients().size() > 0)
						targetChannel.setOpFd(targetChannel.getClients().front()->getUserSock());
					break;
				}
			}
			// client가 접속해있는 채널 리스트에서 타겟삭제
			for (std::vector<Channel *>::iterator it = client.getJoinedChannel().begin(); it != client.getJoinedChannel().end(); ++it){
				if ((*it)->getChannelName() == targetChannel.getChannelName()){
					client.getJoinedChannel().erase(it);
					break;
				}
			}			
			// 타겟 채널에 아무도 없으면 채널삭제
			if (targetChannel.getClients().size() == 0)
				server.getChannels().erase(targetChannel.getChannelName());
		} 
	}
	return;
}
bool  Part::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	(void) cmdlist;
	(void) client;
	return true;
}
