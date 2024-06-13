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
		std::map<std::string, Channel> &channelList = _server.getChannelList();
		std::map<std::string, Channel>::iterator channelIt = channelList.find(*iter);
		// 채널이 존재 할 경우
		if (channelIt != channelList.end())
		{
			Channel channel = channelIt->second;
			// 채널에 클라이언트가 있는지 확인
			if (channel.diffClientInChannel(fd))
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
			if (channel.diffMode('i'))
			{
				if (channel.diffInvite(fd))
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
			if (channel.diffMode('k'))
			{
				if (cmdVector.size() <= 2 || keyIter == joinKeyArgv.end() || !channel.diffKey(*keyIter))
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
			if (channel.diffMode('l'))
			{
				if (channel.getFdListClient().size() >= channel.getLimit()) // LIMIT을 초과할 때 :
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
			// 채널에 클라이언트 추가
			channel.appendFdListClient(fd);
			// 채널에 클라이언트 추가 메시지 전송
			channel.messageAllChannel(fd, "JOIN", *iter, ", ");
		}
		else // 채널이 존재하지 않을 경우
		{
			Channel channel = Channel(*iter, fd);
			// channelList.insert(std::make_pair(*iter, &channel));
			channelList.insert(std::make_pair(*iter, channel));
			channel.appendFdListClient(fd);
			channel.messageAllChannel(fd, "JOIN", *iter, ", ");
			iter++;
			if (cmdVector.size() > 2 || keyIter != joinKeyArgv.end())
			{
				keyIter++;
			}
		}
	}
}