#include "../includes/Command.hpp"

void Command::join(int fd, std::vector<std::string> cmdVector)
{
	Client &client = _server.getClientList().find(fd)->second;
	if (cmdVector.size() < 2) // 명령어에 인자가 부족할 때 :
	{
		// ERR_NEEDMOREPARAMS = ":<server> 461 <nickname> JOIN :Not enough parameters"
		ERROR_needmoreparams_461(client);
		return;
	}

	// parse <channel> : <joinChannelArgv>
	std::vector<std::string> joinChannelArgv = split(cmdVector[1], ','); // #general,#random
	std::vector<std::string>::iterator iter = joinChannelArgv.begin();

	// parse <options> : <key>
	std::vector<std::string> joinKeyArgv;
	std::vector<std::string>::iterator keyIter;

	// optional
	if (cmdVector.size() > 2) // JOIN <#channel> <key> 3개 이상인 경우 ** JOIN은 key만 argv 로 올 수 있음! **
	{	//#general, #seok 등 채널이 2개 이상인 경우, key도 2개 이상 올 수 있음
		joinKeyArgv = split(cmdVector[2], ',');
		keyIter = joinKeyArgv.begin();
		return;
	}

	while (iter != joinChannelArgv.end())
	{
		// 채널 이름이 # 또는 &로 시작하지 않는 경우
		// if (iter->at(0) != '#' && iter->at(0) != '&')
		if ((*iter)[0] != '#' && (*iter)[0] != '&')
		{
			ERROR_nosuchchannel_403(client, *iter);
			iter++;
			if (cmdVector.size() > 2 && keyIter != joinKeyArgv.end())
				keyIter++;
			continue;
		}
		std::map<std::string, Channel*> &channelList = _server.getChannelList();
		std::map<std::string, Channel*>::iterator channelIt = channelList.find(*iter);
		// 채널이 존재 할 경우
		if (channelIt != channelList.end())
		{
			Channel *channel = channelIt->second;
			// 채널에 클라이언트가 있는지 확인
			if (channel->diffClientInChannel(fd))
			{
				// 이미 채널에 있는 경우
				iter++;
				// optional
				if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
				{
					keyIter++;
				}
				continue;
			}
			// invite mode 일 때
			if (channel->diffMode('i'))
			{
				if (channel->diffInvite(fd))
				{
					ERROR_inviteonlychan_473(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						// key처리
						keyIter++;
					}
					continue;
				}
			}
			// key mode 일 때
			if (channel->diffMode('k'))
			{
				if (cmdVector.size() <= 2 || keyIter == joinKeyArgv.end() || !channel->diffKey(*keyIter))
				{
					// ERR_BADCHANNELKEY = ":<server> 475 <nickname> <channel> :Cannot join channel (+k)"
					ERROR_badchannelkey_475(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						keyIter++;
					}
					continue;
				}
			}
			// LIMIT 모드일 때
			if (channel->diffMode('l'))
			{
				if (channel->getFdListClient().size() >= channel->getLimit()) // LIMIT을 초과할 때 :
				{
					// ERR_CHANNELISFULL = ":<server> 471 <nickname> <channel> :Cannot join channel (+l)"
					ERROR_channelisfull_471(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						// KEY 처리
						keyIter++;
					}
					continue;
				}
			}
			std::string channelName = (*channelIt).second->getChannelName();
			client.appendChannelList(channelName);					// operator인 client가 속한 channelList에 '#genral, #random' 추가
			(*channelIt).second->appendFdListClient(fd);			// #general, #random 등 실제 채널에 fd 추가 (이름으로 직접 접근 ㄴㄴ 서버에 저장된 채널리스트를 iter로 순회하며 채널 접근)
			messageAllChannel(fd, "JOIN", channelName, ", ");		// 채널에 JOIN 메시지 전송 -> "야 채널에 누구 참여했다"
			topicMessage(fd, channelName);								// fd에게 채널's TOPIC 메시지 전송 -> "야 신삥 우리 채널에 주제는 ~ 라고 해"
		}
		else // 채널이 존재하지 않을 경우
		{
			_server.appendNewChannel(fd, *iter);					// 채널 없으면 생성
			_server.findChannel(*iter)->appendFdListClient(-1);		// -1은 봇 예정 -> 봇이 1빠 예정
			_server.findChannel(*iter)->appendFdListClient(fd);		// 새로 만들어진 채널's 클라이언트 리스트 중 클라이언트(fd주인) 추가
			client.appendChannelList(*iter);						// client가 속한 channelList에 '#채널' 추가
			messageAllChannel(fd, *iter, "JOIN", "");
		}
		nameListMsg(fd, *iter);
		iter++;
		if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
			keyIter++;
	}
}