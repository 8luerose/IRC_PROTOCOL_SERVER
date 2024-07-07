#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::kick(int fd, std::vector<std::string> cmdVector)
{
	// KICK <channel> <nickname>

	std::map<int, Client>& clientList = _server.getClientList();			// 서버에 접속한 클라이언트 리스트
	std::map<int, Client>::iterator operator_iter = clientList.find(fd);	// 클라이언트 리스트에서 '명령어를 사용'하려는 클라이언트의 fd를 찾아 반환
	if (cmdVector.size() < 3)												// "KICK, <channel>, <nickname>" 안 들어올 경우
	{
		ERROR_needmoreparams_461(operator_iter->second);
		return;
	}

	std::stringstream ss(cmdVector[1]);
	std::string buffer;
	std::vector<std::string> channelVector;
	while (getline(ss, buffer, ','))
	{
		channelVector.push_back(buffer);
	}	// ex) #channel1, #channel2, #channel3

	// ==== 채널마다 oper 권한있는지 확인 ====
	std::vector<std::string>::iterator channel_iter;
	for (channel_iter = channelVector.begin(); channel_iter != channelVector.end(); channel_iter++)
	{
		Channel *channel = _server.findChannel(*channel_iter);					// 채널을 찾아 반환
		if (channel && !channel->diffOperator(fd))								// 채널이 존재하고, 채널의 operator가 아닐 경우
		{
			ERROR_chanoprivsneeded_482(operator_iter->second, *channel_iter);
			return;
		}
	}
	// ==== 통과 ====


	std::stringstream nickss(cmdVector[2]);
	std::string nickbuffer;
	std::vector<std::string> nickVector;
	while (getline(nickss, nickbuffer, ','))
	{
		nickVector.push_back(nickbuffer);
	}
	std::vector<std::string>::iterator nickIter;

	nickIter = nickVector.begin();
	// ==== 채널 kick ====
	for (channel_iter = channelVector.begin(); channel_iter != channelVector.end(); channel_iter++)
	{
		Channel *channel = _server.findChannel(*channel_iter);					// 채널을 찾아 반환

		if (channel == NULL)													// 채널이 존재하지 않을 경우
			ERROR_nosuchchannel_403(operator_iter->second, *channel_iter);
		else
		{
			std::map<int, Client>::iterator target = _server.findClient(*nickIter);	// <nickname> 찾아 반환

			// ==== <nickname> 인가 확인 ====
			if (target == _server.getClientList().end())								// <nickname> 서버에 존재하지 않을 경우
			{
				ERROR_nosuchnick_401(operator_iter->second, *nickIter);
				return;
			}
			if (target->second.getClientFd() == -1)										// <nickname> Client::resetclient()일 경우
			{
				ERROR_nosuchnick_401(operator_iter->second, *nickIter);
				return;
			}
			if (target->second.getNickname() == operator_iter->second.getNickname())	// <nickname> 자신일 경우
			{
				return;
			}
			// ==== 통과 ====

			// ==== 본격적으로 kick ====
			else
			{
				if (channel->diffClientInChannel(target->second.getClientFd()) == false)	// 찾고자 하는 채널에 <nickname>이 존재하지 않을 경우
					ERROR_usernotinchannel_441(operator_iter->second, *nickIter, *channel_iter);

				else
				{
					std::string message = *nickIter;
					if (cmdVector.size() > 3)
						message += " " + cmdVector[3];

					messageAllChannel(fd, *channel_iter, "KICK", message);		// 채널에 있는 모든 클라이언트에게 메시지 전송
					channel->removeFdListClient(target->second.getClientFd());	// '채널이 갖고 있는' 클라이언트 리스트에서 해당 nickname 클라이언트 제거
					channel->removeOperatorFd(target->second.getClientFd());	// '채널이 갖고 있는' 클라이언트 리스트에서 해당 nickname 클라이언트 제거
					target->second.deleteChannel(*channel_iter);				// '클라이언트가 갖고 있는' std::vector<std::string> _channelList; 에서 해당 채널 '_channelList.erase(iter);'
				}
			}
		}
		nickIter++;
	}
	// ==== kick 끝 ==== 
}