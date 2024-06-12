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
{
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