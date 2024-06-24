#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::quit(int fd, std::vector<std::string> cmdVector)
{	// 양식: QUIT <message>
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator clientIter;

	clientIter = clientList.find(fd);								// 서버 clientList(서버 참여자 목록)에서 fd를 찾는다
	if (clientIter == clientList.end())								// 만약 fd가 clientList에 없다면
		return ;													// 함수 종료
	
	// 메시지 구성 종류는 총 3가지임 - tmi
	// (1) 정상종료: A client session is ended with a quit message -> "QUIT <message>"
	// (2) 서버간 연결 끊김: netsplits (disconnecting of two servers) occur ->  "composed of the names of two servers involved.."
	// (3) 클라이언트 이슈: client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs on socket) -> "message  reflecting the nature of the event which caused it to happen"
	
	// Command - quit() 에서는 (1) 정상종료만 다룰거임
	std::vector<std::string> channelList;
	channelList = clientIter->second.getChannelList();				// 클라이언트(fd)가 참여한 '채널'들' 목록'을 가져옴 -> 왜? 클라이언트 private에 clientList이 있어서 가능
	std::vector<std::string>::iterator channelIter;
	
	channelIter = channelList.begin();
	while (channelIter != channelList.end())
	{	// 클라이언트가 참여한 채널 목록을 순회할거임
		Channel *channel = _server.findChannel(*channelIter);		// 채널 목록'들' 중 채널 하나
		
		// ==== 채널 유효성 검사 ====
		if (channel == NULL)										// 호오옥시라도 만약 채널이 없다면: 엥? 왜 혹시임? -> clientIter->second.getChannelList() 클라이언트에서 가져왔자너~ 업뎃안됐을수도?
			continue ;												// 다음 채널로 넘어가~
		// ==== 통과 ====

		channel->removeFdListClient(fd);							// 해당 채널에서 클라이언트(fd)를 제거
		channel->removeOperatorFd(fd);								// 해당 채널에서 클라이언트(fd)를 오퍼레이터에서 제거 -> 어차피 없으면 "_operatorFdList.end()" 반환됨 ㄱㅊㄱㅊ
		messageAllChannel(fd, channel->getChannelName(), "QUIT", channelMessage(1, cmdVector));	// 채널에 메시지를 보냄 -> cmdVector[1]은 <message>
		// ex) " : name ! user @ host QUIT #channel : message \r\n"
		
		if (channel->getFdListClient().size() == 1)					// 만약 채널에 남은 클라이언트가 1명이라면
		{
			_server.removeChannel(channel->getChannelName());		// 서버's 채널 목록에서 <#채널이름> 해당 채널 제거
			delete channel;											// 채널 메모리 해제
			// Server::appendNewChannel(int fd, std::string& channelName) 에서
 			// ( ... , 'new Channel'(channelName, fd)));
			// new로 동적할당했기 때문에 Channel 은 꼭 delete 할것!!
		}
		channelIter++;												// 다음 채널로 넘어감
	}
	clientIter->second.resetClient();								// 클라이언트 정보 초기화 -> 다른 클라이언트 또 쓸 수 있게
	clientList.erase(fd);											// 서버's 클라이언트 목록에서 해당 클라이언트(fd) 제거
	close(fd);														// fd 닫기 -> 클라이언트 소켓 닫기
}