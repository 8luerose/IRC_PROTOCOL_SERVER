#include "../includes/Command.hpp"


void Command::nick(int fd, std::vector<std::string> cmdVector)
{
	// "/NICK <nickname>"
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator iter;
	std::vector<std::string> channelList;
	std::vector<std::string>::iterator channelIter;

	// ==== 인가 확인 작업 ====
	iter = clientList.find(fd);
	if (!(iter->second.getRegiPass()))	// Client가 pass 인증이 안되어있으면
	{
		ERROR_notregistered_451(iter->second);
		send(fd, iter->second.getReciveBuf().c_str(), iter->second.getReciveBuf().length(), 0);
		iter->second.resetClient();	// 그 클라이언트 상태 초기화
		clientList.erase(fd);		// 클라이언트 리스트에서 삭제
		close(fd);					// 그 클라이언트 소켓 닫기
		return;
	}

	if (cmdVector.size() < 2)
	{
		ERROR_nonicknamegiven_431(iter->second);
		return;
	}

	while (1)	// 닉네임이 중복되지 않도록 처리
	{
		std::map<int, Client>::iterator findIter;
		
		findIter = _server.findClient(cmdVector[1]);
		if (findIter != _server.getClientList().end())
			cmdVector[1] = findIter->second.getNickname() + "_";
		else
			break ;
	}

	if (nickNameValidate(cmdVector[1]) == false)
	{
		ERROR_erroneusnickname_432(iter->second);
		iter->second.appendReciveBuf("/NICK <nickname> First Letter is not digit and length is under 10.\r\n");
		return;
	}
	
	if (nicknameDuplicate(cmdVector[1], clientList) == false)
	{
		ERROR_nicknameinuse_433(iter->second);
		return;
	}
	// ==== 통과 ====

	// ==== 닉네임 변경 로직 ====
	std::string oldNick = iter->second.getNickname();
	channelList = iter->second.getChannelList();
	channelIter = channelList.begin();
	_sendNickList.clear();

	if (channelList.size() == 0)		// 아무런 채널에 가입되어있지 않은 경우
	{
		std::string prefix;
		prefix = (":" + oldNick + "!" + iter->second.getUsername() + "@" + iter->second.getServername());
		iter->second.appendReciveBuf(prefix + " " + cmdVector[0] + " :" + cmdVector[1] + "\r\n");
	}
	else								// 채널에 1개 이상 가입되어 있는 경우
	{
		while (channelIter != channelList.end())
		{
			std::cout << "#채널 Iter 순회->" << std::endl;
			Channel* channel = _server.findChannel(*channelIter);
			if (channel != NULL)
			{
				messageAllChannel(fd, channel->getChannelName(), "NICK", cmdVector[1]);
			}
			channelIter++;
		}
	}
	iter->second.setNickname(cmdVector[1]);	// iter == clientList의 iter
	iter->second.setRegiNick(true);
}

bool Command::nickNameValidate(std::string nickname)
{
	if (nickname.length() == 0)
		return (false);
	if (nickname.length() > 9)
		return (false);
	if (nickname[0] >= '0' && nickname[0] <= '9')
		return (false);
	for (size_t i = 1; i < nickname.length(); i++)	// 첫글자는 숫자가 아니어야 하고, 길이는 9이하
	{
		if (!isalnum(nickname[i]) && !isSpecialChar(nickname[i]))
			return (false);
	}
	return (true);
}

bool Command::nicknameDuplicate(std::string nickname, std::map<int, Client>& clientList)
{
	std::map<int, Client>::iterator iter;

	iter = clientList.begin();
	while (iter != clientList.end())
	{
		if (iter->second.getNickname() == nickname)
			return (false);
		iter++;
	}
	return (true);
	std::cout << "#nick중복" << std::endl;
}
