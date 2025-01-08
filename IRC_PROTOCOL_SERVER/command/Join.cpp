#include "../includes/Command.hpp"

void Command::join(int fd, std::vector<std::string> cmdVector)
{
	// /JOIN <#channel> [<key>]
	std::cout << "#JOIN" << std::endl;
	std::cout << "#cmdVector.size() : " << cmdVector.size() << std::endl;

	Client &client = _server.getClientList().find(fd)->second;
	if (cmdVector.size() < 2)
	{
		ERROR_needmoreparams_461(client);
		return;
	}

	std::vector<std::string> joinChannelArgv;
	std::vector<std::string>::iterator iter;
	if (cmdVector[1].find(',') != std::string::npos)
	{
		joinChannelArgv= split(cmdVector[1], ',');
		iter = joinChannelArgv.begin();
		std::cout << "#Channel 2개" << std::endl;
	}
	else
	{
		joinChannelArgv.push_back(cmdVector[1]);
		iter = joinChannelArgv.begin();
		std::cout << "#Channel 1개" << std::endl;
	}

	std::vector<std::string> joinKeyArgv;
	std::vector<std::string>::iterator keyIter;

	if (joinChannelArgv.size() > 1)
	{
		std::cout << "#Channel 2개 경우" << std::endl;
		if(cmdVector.size() >= 3)
		{
			if (cmdVector[2] == "")
			{
				joinKeyArgv.push_back("");
			}
			else
			{
				joinKeyArgv = split(cmdVector[2], ',');
			}
			keyIter = joinKeyArgv.begin();
			std::cout << "#Channel 2개 + Key 2개" << std::endl;
			std::cout << "#key = " << *keyIter << std::endl;
		}
	}
	else	// JOIN <#channel> <key> 중 key는 keyIter로 지정해야 함
	{
		std::cout << "#Channel 1개 경우" << std::endl;
		if (cmdVector.size() >= 3)	// JOIN <#channel> <key> 3개 이상인 경우
		{
			joinKeyArgv.push_back(cmdVector[2]);	// key를 joinKeyArgv에 추가
			keyIter = joinKeyArgv.begin();
			std::cout << "#Channel 1개 + Key 1개" << std::endl;
			std::cout << "#key = " << *keyIter << std::endl;
		}
		else						// JOIN <#channel> <' '> -> key가 없는 경우
		{
			joinKeyArgv.push_back("");
			keyIter = joinKeyArgv.begin();
			std::cout << "#Channel 1개 + Key 0개" << std::endl;
		}
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
		std::cout << "#채널 이름 인가 통과" << std::endl;
		std::map<std::string, Channel*> &channelList = _server.getChannelList();
		std::map<std::string, Channel*>::iterator channelIt = channelList.find(*iter);
		// 채널이 존재 할 경우
		if (channelIt != channelList.end())
		{
			std::cout << "#채널이 존재함" << std::endl;
			Channel *channel = channelIt->second;
			// 채널에 클라이언트가 있는지 확인
			if (channel->diffClientInChannel(fd))
			{
				ERROR_useronchannel_443(client, client.getNickname(), channel->getChannelName());

				std::cout << "#채널에 클라이언트가 이미 있음" << std::endl;
				// 이미 채널에 있는 경우
				iter++;
				if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
				{
					keyIter++;
				}
				continue;
			}
			std::cout << "#채널에 클라이언트가 없음" << std::endl;
			if (channel->diffMode('k'))
			{
				std::cout << "#join에서 key 체크" << std::endl;
				if (joinKeyArgv.empty())
				{
					ERROR_badchannelkey_475(client, *iter);
					return ;
				}
				else if (cmdVector.size() <= 2 || keyIter == joinKeyArgv.end() || (keyIter != joinKeyArgv.end() && channel->diffKey(*keyIter) == false))	// 태현 수정, key가 없거나 key가 틀릴 때
				{
					ERROR_badchannelkey_475(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						keyIter++;
					}
					continue;
				}
			}
			std::cout << "#key 체크 통과" << std::endl;
			// invite mode 일 때
			if (channel->diffMode('i'))
			{
				std::cout << "#join에서 invite 체크" << std::endl;
				if (channel->diffInvite(fd) == false)	
				{
					ERROR_inviteonlychan_473(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						keyIter++;
					}
					continue;
				}
			}
			std::cout << "#invite 체크 통과" << std::endl;
			// LIMIT 모드일 때
			if (channel->diffMode('l'))
			{
				std::cout << "#join에서 limit 체크" << std::endl;
				if (channel->getFdListClient().size() >= channel->getLimit())
				{
					ERROR_channelisfull_471(client, *iter);
					iter++;
					if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
					{
						keyIter++;
					}
					continue;
				}
			}
			std::cout << "#limit 체크 통과" << std::endl;
			std::string channelName = (*channelIt).second->getChannelName();
			client.appendChannelList(channelName);						// operator인 client가 속한 channelList에 '#genral, #random' 추가
			(*channelIt).second->appendFdListClient(fd);				// #general, #random 등 실제 채널에 fd 추가 (이름으로 직접 접근 ㄴㄴ 서버에 저장된 채널리스트를 iter로 순회하며 채널 접근)
			messageAllChannel(fd, channelName, "JOIN", "");				// 채널에 JOIN 메시지 전송 -> "야 채널에 누구 참여했다"
			topicMessage(fd, channelName);								// fd에게 채널's TOPIC 메시지 전송 -> "야 신삥 우리 채널에 주제는 ~ 라고 해"
		}
		else // 채널이 존재하지 않을 경우
		{
			std::cout << "#채널이 존재하지 않음" << std::endl;
			_server.appendNewChannel(fd, *iter);					// 채널 없으면 생성
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