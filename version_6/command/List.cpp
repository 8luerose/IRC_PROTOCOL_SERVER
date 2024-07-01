#include "../includes/Command.hpp"

void Command::list(int fd, std::vector<std::string> cmdVector)
{
	// LIST [<channel>{,<channel>} [<server>]]
	// LIST 명령어는 채널 목록과 토픽을 보여준다
	// server는 생략 가능하다
    std::map<std::string, Channel*>& channelList = _server.getChannelList();
    std::map<int, Client>& clientList = _server.getClientList();
    std::map<int, Client>::iterator clientIt = clientList.find(fd);

    if (clientIt == clientList.end())	// fd 클라이언트가 클라이언트 리스트에 존재하지 않을 경우
        return;

    Client &client = clientIt->second;	// map<int, Client>에서 fd에 해당하는 Client 객체 반환


	// 디버깅
	std::cout << "#cmdVector.size() : " << cmdVector.size() << std::endl;
	std::cout << "#cmdVector: ";
	for (std::vector<std::string>::iterator it = cmdVector.begin(); it != cmdVector.end(); it++)
	{
		std::cout << *it << ".";
	}
	std::cout << std::endl;

    // RPL_LISTSTART (321)
    client.appendReciveBuf("321 " + client.getNickname() + " Channel :Users Name\r\n");


	// // "list -yes" 일 경우
	// if (cmdVector.size() >= 2 && (cmdVector[1] == "-yes" || cmdVector[1] == "-YES" || cmdVector[1] == "-y" || cmdVector[1] == "-Y"))
	// {
	// 	std::cout << "#LIST -YES" << std::endl;
	// 	std::map<std::string, Channel*>::iterator channelIt = channelList.begin();
	// 	while (channelIt != channelList.end())
	// 	{
	// 		Channel* channel = channelIt->second;
	// 		std::string channelName = channel->getChannelName();
	// 		std::string topic = channel->getTopic();
	// 		client.appendReciveBuf("322 " + client.getNickname() + " " + channelName + " " + intToString(channel->getFdListClient().size()) + " :" + topic + "\r\n");
	// 		channelIt++;
	// 	}
	// }

	// 채널 2개 이상일 때
    if (cmdVector.size() >= 2 && cmdVector[1] != "")
    {	// 채널 여러개일 때: #channel1,#channel2
		std::cout << "#LIST <channel>{,<channel>} [<server>]" << std::endl;
        std::vector<std::string> channelNames = split(cmdVector[1], ',');
		std::vector <std::string>::iterator it = channelNames.begin();
		while (it != channelNames.end())
		{	// #channel1, #channel2, ... 채널 순회
			std::cout << "#t1 " << *it << std::endl;
			std::map<std::string, Channel*>::iterator channelIt = channelList.find(*it);	// 채널 리스트에서 #channel 채널 찾기
			if (channelIt != channelList.end())
			{
				Channel* channel = channelIt->second;
				std::string channelName = channel->getChannelName();
				std::string topic = channel->getTopic();
				client.appendReciveBuf("322 " + client.getNickname() + " " + channelName + " " + intToString(channel->getFdListClient().size()) + " :" + topic + "\r\n");
				// ex) 322 닉네임 #channel 인원수 :토픽
			}
			it++;
		}
    }
    else
    {
		std::cout << "#LIST 1 channel" << std::endl;
		std::map<std::string, Channel*>::iterator channelIt = channelList.begin();
		while (channelIt != channelList.end())
		{
			std::cout << "#t2 " << channelIt->first << std::endl;
			Channel* channel = channelIt->second;
			std::string channelName = channel->getChannelName();
			std::string topic = channel->getTopic();
			client.appendReciveBuf("322 " + client.getNickname() + " " + channelName + " " + intToString(channel->getFdListClient().size()) + " :" + topic + "\r\n");
			channelIt++;
		}
    }

    // RPL_LISTEND (323)
    client.appendReciveBuf("323 " + client.getNickname() + " :End of /LIST\r\n");
    send(fd, client.getReciveBuf().c_str(), client.getReciveBuf().length(), 0);
    client.clearReciveBuf();
}
