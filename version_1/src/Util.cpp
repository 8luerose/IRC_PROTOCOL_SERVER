#include "../includes/Util.hpp"

bool isSpecialChar(char c)
{
	if (c == '_' || c == '|' || c == '[' || c == ']' \ 
		|| c == '{' || c == '}' || c == '\\')
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
	std::map<std::string, Channel>& channelList = _server.getChannelList();

	if (channelList.find(channelName) == channelList.end())
		return ;
	Channel& channel = channelList.find(channelName)->second;
	std::vector<int> fdList = channel.getFdListClient();
	std::vector<int>::iterator fd_iter = fdList.begin();
	while (fd_iter != fdList.end())
	{
		Client& targetClient = _server.getClientList().find(*fd_iter)->second;
		if ((fd == (*fd_iter)) && (command == "PRIVMSG")) // 자신에게는 보내지 않음
		{
			fd_iter++;
			continue ;
		}
		targetClient.appendReciveBuf(makeMsgForm(fd) + " " + command + " " + channelName + " :" + message + "\r\n");
		// prefix = (":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername());
		// <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
		// == " : name ! user @ host PRIVMSG #channel : message \r\n"

		// 2.3.1 Message format in 'pseudo' BNF -> [page 8]

		fd_iter++;
	}
}

std::string Command::channelMessage(int index, std::vector<std::string> command_vec)
{
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
