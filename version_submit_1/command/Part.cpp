#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::part(int fd, std::vector<std::string> cmdVector)
{
	// PART <channel>
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator fd_iter;

	fd_iter = clientList.find(fd);
	if (cmdVector.size() < 2)
	{
		ERROR_needmoreparams_461(fd_iter->second);
		return;
	}

	std::stringstream ss(cmdVector[1]);			// <channel>{,<channel>}
	std::string buffer;
	std::vector<std::string> channelVector;
	while (getline(ss, buffer, ','))			//	<channel> <channel> <channel> ... 담는다
		channelVector.push_back(buffer);
	
	std::vector<std::string>::iterator channelVector_iter;
	channelVector_iter = channelVector.begin();

	while (channelVector_iter != channelVector.end())
	{
		std::vector<std::string>::iterator channelList_iter;
		channelList_iter = fd_iter->second.findChannel(*channelVector_iter);	// 입력받은 채널 #general #random .. '채널'에 해당하는 iter 위치 반환
		if (channelList_iter != fd_iter->second.getChannelList().end())			// 만약, 채널에 있으면
		{
			Channel *channel = _server.findChannel(*channelList_iter);			// 채널 iter로 채널 본연의 Channel *channel 반환
			messageAllChannel(fd, *channelList_iter, "PART", channelMessage(2, cmdVector));
			
			channel->removeFdListClient(fd);									// 채널 인원 목록에서 fd 제거
			channel->removeOperatorFd(fd);										// 채널 오퍼레이터 목록에서 fd 제거
			fd_iter->second.deleteChannel(*channelList_iter);					// 클라이언트가 속한 채널 List에서 해당 채널 제거
			
			if (channel->getFdListClient().size() == 0)							// 만약 해당 채널에 남은 인원이 1명이라면
			{
				_server.removeChannel(channel->getChannelName());				// 서버에서 채널리스트 중 해당 채널 제거
				delete channel;
 				// Server::appendNewChannel(int fd, std::string& channelName) 에서
 				// ( ... , 'new Channel'(channelName, fd)));
				// new로 동적할당했기 때문에 Channel 은 꼭 delete 할것!!
			}
		}
		else	//만약 채널에 fd가 없는데 시도할 경우
		{
			if (_server.findChannel(*channelVector_iter))						// #channel가 존재는 할 경우
				ERROR_notonchannel_442(fd_iter->second, *channelVector_iter);	// ERR_NOTONCHANNEL "You're not on that channel\r\n"
			else																// #channel자체가 존재하지 않을 경우
				ERROR_nosuchchannel_403(fd_iter->second, *channelVector_iter);	// ERR_NOSUCHCHANNEL "No such channel\r\n"
		}
		channelVector_iter++;
	}
}