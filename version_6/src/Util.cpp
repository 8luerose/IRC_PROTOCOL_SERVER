#include "../includes/Util.hpp"

bool isSpecialChar(char c)
{
	if (c == '_' || c == '|' || c == '[' || c == ']' || c == '{' || c == '}' || c == '\\')
	{
		return (true);
	}
	return (false);
}

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string> tmpVector;
	std::stringstream ss;
	std::string buffer;

	ss << str;
	while (getline(ss, buffer, delim))
	{
		tmpVector.push_back(buffer);
	}
	return (tmpVector);
}

std::string Command::makeMsgForm(int fd)
{	// 접두사 구문 함수
	// ex) :<닉네임>!<사용자 아이디>@<호스트네임>
	
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator iter = clientList.find(fd);
	Client &client = iter->second;
	std::string prefix;
	
	prefix = (":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername());
	// <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
	// 2.3.1 Message format in 'pseudo' BNF -> [page 8]
	return (prefix);
}

void Command::messageAllChannel(int fd, std::string channelName, std::string command, std::string message)
{	// 채널 전체에 귓속말, PRIVMSG 중에서 reciver -> channel인 경우
	// 다만 자신에게는 PRIVMSG 내용이 출력되지 않음
	// 입력 인자는, /PRIVMSG #channel :message
	std::map<std::string, Channel*>& channelList = _server.getChannelList();

	if (channelList.find(channelName) == channelList.end())
		return ;
	Channel *channel = channelList.find(channelName)->second;
	std::vector<int> fdList = channel->getFdListClient();
	std::vector<int>::iterator fd_iter = fdList.begin();
	while (fd_iter != fdList.end())
	{
		std::cout << "#fd_iter : " << *fd_iter << std::endl;
		Client& targetClient = _server.getClientList().find(*fd_iter)->second;
		if ((fd == (*fd_iter)) && (command == "PRIVMSG")) // 자신에게는 보내지 않음
		{
			fd_iter++;
			continue ;
		}
		std::cout << "#targetClient.getNickname() : " << targetClient.getNickname() << std::endl;
		targetClient.appendReciveBuf(makeMsgForm(fd) + " " + command + " " + channelName + " :" + message + "\r\n");
		std::cout << "#targetClient.getReciveBuf() : " << targetClient.getReciveBuf() << std::endl;
		// prefix = (":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getServername());
		// <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
		// == " : name ! user @ host PRIVMSG #channel : message \r\n"
		// 2.3.1 Message format in 'pseudo' BNF -> [page 8]
		fd_iter++;
	}
}

std::string Command::channelMessage(int index, std::vector<std::string> command_vec)
{	// 채널 메시지란? :<메시지> 이런 형태로 출력
	std::string message = "";
	// 커멘드 벡터의 인덱스부터 메시지 생성
	for (size_t i = index; i < command_vec.size(); i++)
	{
		// 메시지에 커멘드 벡터의 인덱스 추가
		message += command_vec[i];
		// 만약 인덱스가 마지막이 아니면 공백 추가
		if (i != command_vec.size() - 1)
			message += " ";
	}
	// 만약 메시지가 ':'로 시작하면 ':' 제거
	if (message[0] == ':')
		message = message.substr(1, message.length() - 1); // ':' 제거
	// 만약 메시지가 비어있으면 NO REASON 추가
	if (message.length() == 0)
		message = "NO REASON"; // NO REASON
	return (message);
}

void Command::topicMessage(int fd, std::string channelName)
{	// 채널의 토픽을 출력하는 함수
	std::map<std::string, Channel *>& channelList = _server.getChannelList();	// 전체 채널 리스트
	Channel *channel = channelList.find(channelName)->second;					// 채널 이름으로 채널 찾기
	
	std::string topic = channel->getTopic();									// 채널의 토픽 가져오기
	
	// ==== 토픽 없으면 ====
	if (topic.length() == 0)
		return;

	// ==== 토픽 있으면 ====
	topic = topic.substr(1, topic.length() - 1);								// ':' 제거
	Client &client = _server.getClientList().find(fd)->second;					//	fd로 클라이언트 찾기
	client.appendReciveBuf("332 " + client.getNickname() + " " + channelName + " :" + topic + "\r\n");	// 토픽 출력
	// 322 <nickname> #channel :<topic>
}

void Command::nameListMsg(int fd, std::string channelName)
{	// 채널의 클라이언트 리스트를 출력하는 함수
	std::map<std::string, Channel *>& channelList = _server.getChannelList();		// 전체 채널 리스트

	// 채널 이름으로 찾아봤는데, .end()면 채널이 없는 것 -> 종료	
	if (channelList.find(channelName) == channelList.end())						
		return;

	Channel *channel = channelList.find(channelName)->second;						// 채널 이름으로 채널 찾기
	std::vector<int> clientFdList = channel->getFdListClient();						// 그 채널에 클라이언트 리스트 찾기
	std::vector<int>::iterator iter;
	std::string message;

	iter = clientFdList.begin();
	while (iter != clientFdList.end())												// #channelName의 클라이언트 리스트 순회
	{
		Client &client = _server.getClientList().find(*iter)->second;				// #channelName에 속한 모든 클라이언트 다 돌거임 (iter++)
		if (channel->diffOperator(client.getClientFd()))							// 만약 오퍼레이터면(방장이면)
			message += "@";
		
		message += client.getNickname();											// 메세지에 닉네임 추가
		
		if (iter != clientFdList.end() - 1)											// 마지막이 아니면 " " 붙여서 또 추가의 여지 남김(while이니까)
			message += " ";
		
		iter++;
	}
	Client &client = _server.getClientList().find(fd)->second;						// 명령어 사용하려는 fd 클라이언트에게
	client.appendReciveBuf("353 " + client.getNickname() + " = " + channelName + " :" + message + "\r\n");	// 채널의 클라이언트 리스트 (== mesaage) 주고
	// 353 <nickname> = #channel :@nickname nickname nickname nickname
	client.appendReciveBuf("366 " + client.getNickname() + " " + channelName + " :End of NAMES list.\r\n");	// 끝내기
	// 366 <nickname> #channel :End of NAMES list.
}

// 태현 추가
std::string Command::intToString(int value)
{
	std::string str;
	std::stringstream ss;

	ss << value;	// 임시 스트림 객체에 int값 넣기
	ss >> str;		// 임시 스트림 객체에서 string으로 변환
	return (str);	// string 반환
}