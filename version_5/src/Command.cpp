#include "../includes/Command.hpp"

Command::Command(Server &server) : _server(server)
{
}

Command::~Command()
{
}

// print cmdVector 석준 추가
void printCmdVector(std::vector<std::string> cmdVector)
{
	std::cout << "cmdVector: ";
	for (std::vector<std::string>::iterator it = cmdVector.begin(); it != cmdVector.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void Command::run(int fd)
{
	std::cout << "#command run" << std::endl;

	std::stringstream				serverMsg;	// server에 저장된 메시지
	std::string						cmdBuffer;
	std::vector<std::string>		cmdVector;
	std::map<int, Client>::iterator	iter;		// clients를 순회하기 위한 iterator
	std::map<int, Client>& clientList = _server.getClientList();	// 서버에 저장된 client 목록
	serverMsg << _server.getMessage(fd);
	// while (getline(serverMsg, cmdBuffer, ' ')) // 명령어 파싱
	// {
	// 	std::size_t pos = cmdBuffer.find_last_not_of("\r\n");
	// 	//find_last_not_of는 특정 문자 집합에 속하지 않는 마지막 문자의 위치를 반환하는 함수
	// 	// Hello, World\r\n -> 'd'의 위치를 반환
	// 	cmdVector.push_back(cmdBuffer.substr(0, pos + 1));
	// 	// "/JOIN #test" -> /JOIN #test 명령어 전체를 Vector에 저장
	// }

	while (getline(serverMsg, cmdBuffer, ' '))	// 명령어 파싱
    {
        size_t pos = cmdBuffer.find_last_not_of("\r\n");
		// find_last_not_of는 특정 문자 집합에 속하지 않는 마지막 문자의 위치를 반환하는 함수
		// Hello, World\r\n -> 'd'의 위치를 반환
        std::string command = cmdBuffer.substr(0, pos + 1);
		// 처음부터 ~ 마지막 문자 (인덱스+1)까지의 문자열을 command에 저장

        if (cmdVector.empty())
        {	// "JOIN #general" 중 -> 처음 "JOIN"만 -> 4글자 for()로 toupper 적용
            for (size_t i = 0; i < command.size(); i++)
                command[i] = std::toupper(command[i]);
        }
        cmdVector.push_back(command);
    }
	printCmdVector(cmdVector);

	iter = clientList.find(fd);
	if (iter == clientList.end())
		return ;
	if ((iter != clientList.end()) && !(iter->second.getIsRegist())) // 클라이언트가 등록되어 있지 않은 경우
	{
		std::cout << "#signUp" << std::endl;
		signUp(fd, iter, cmdVector, clientList);
	}
	else	// 클라이언트가 등록되어 있는 경우
	{
		std::cout << "#signIn" << std::endl;
		signIn(fd, cmdVector);
	}
}

void Command::signUp(int fd, std::map<int, Client>::iterator iter, std::vector<std::string>& cmdVector, std::map<int, Client>& clientList)
{

	if (cmdVector.size() > 0)
	{
		if (cmdVector[0] == "PASS")
		{
			pass(fd, cmdVector);
		}
		else if (cmdVector[0] == "NICK")
		{
			nick(fd, cmdVector);
		}
		else if (cmdVector[0] == "USER")
		{
			user(fd, cmdVector);
		}
		else
		{
			notRegister(fd, iter, clientList);
		}
	}
	// if (cmdVector[0] == "PASS")
	// {
	// 	pass(fd, cmdVector);
	// }
	// else if (cmdVector[0] == "NICK")
	// {
	// 	nick(fd, cmdVector);
	// }
	// else if (cmdVector[0] == "USER")
	// {
	// 	user(fd, cmdVector);
	// }
	// else
	// {
	// 	notRegister(fd, iter, clientList);
	// 	std::cout << "#noregi# tae: " << tae << std::endl;
	// }

	// 첫 실행 시, 인사말 출력
	iter = clientList.find(fd);
	if (iter != clientList.end())
	{
		if (iter->second.getIsRegist())
			iter->second.appendReciveBuf(":IRC 001 " + iter->second.getNickname() + " :Welcome to the Interget Relay Network " + iter->second.getNickname() + "!" + iter->second.getUsername() + "@" + iter->second.getHostname() + "\r\n");
	}
}

void Command::notRegister(int fd, std::map<int, Client>::iterator iter, std::map<int, Client>& clientList)
{
	// ERR_NOTREGISTERED (451):
	// ex) ":server_name 451 <nickname> :You have not registered"

	iter->second.appendReciveBuf(iter->second.getNickname() + " :");
	iter->second.appendReciveBuf(ERR_NOTREGISTERED);
	send(fd, iter->second.getReciveBuf().c_str(), iter->second.getReciveBuf().length(), 0);
	iter->second.resetClient();
	clientList.erase(fd);
	close(fd);


	// RFC1459:	2.3.1
		// <message> ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
		// <prefix> ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
		// <command> ::= <letter> { <letter> } | <number> <number> <number>
		// <params> ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
		// <trailing> ::= <Any, possibly *empty*, sequence of octets not including NUL or CR or LF>
		// <crlf> ::= CR LF
}

void Command::signIn(int fd, std::vector<std::string>& cmdVector)
{
	if (cmdVector.size() > 0)
	{
		if (cmdVector[0] == "USER")
			user(fd, cmdVector);
		else if (cmdVector[0] == "NICK")
			nick(fd, cmdVector);
		else if (cmdVector[0] == "PASS")
			pass(fd, cmdVector);
		else if (cmdVector[0] == "PRIVMSG")
			privmsg(fd, cmdVector);
		else if (cmdVector[0] == "QUIT")
			quit(fd, cmdVector);
		else if (cmdVector[0] == "PART")
			part(fd, cmdVector);
		else if (cmdVector[0] == "JOIN")
			join(fd, cmdVector);
		else if (cmdVector[0] == "KICK")
			kick(fd, cmdVector);
		else if (cmdVector[0] == "MODE")
			mode(fd, cmdVector);
		else if (cmdVector[0] == "TOPIC")
			topic(fd, cmdVector);
		else if (cmdVector[0] == "INVITE")
			invite(fd, cmdVector);
		else if (cmdVector[0] == "LIST")
			list(fd, cmdVector);
		else if (cmdVector[0] == "PING")
			ping(fd, cmdVector);
		else
        {
            // 등록되어 있지 않은 명령어의 경우 에러처리
            std::map<int, Client>& clientList = _server.getClientList();
            std::map<int, Client>::iterator iter = clientList.find(fd);
            if (iter != clientList.end())
                ERROR_unknowncommand_421(iter->second, cmdVector[0]);
        }
	}
	// if (cmdVector[0] == "USER")
	// 	user(fd, cmdVector);
	// else if (cmdVector[0] == "NICK")
	// 	nick(fd, cmdVector);
	// else if (cmdVector[0] == "PASS")
	// 	pass(fd, cmdVector);
	// else if (cmdVector[0] == "PRIVMSG")
	// 	privmsg(fd, cmdVector);
	// else if (cmdVector[0] == "QUIT")
	// 	quit(fd, cmdVector);
	// else if (cmdVector[0] == "PART")
	// 	part(fd, cmdVector);
	// else if (cmdVector[0] == "JOIN")
	// 	join(fd, cmdVector);
	// else if (cmdVector[0] == "KICK")
	// 	kick(fd, cmdVector);
	// else if (cmdVector[0] == "MODE")
	// 	mode(fd, cmdVector);
	// else if (cmdVector[0] == "TOPIC")
	// 	topic(fd, cmdVector);
	// else if (cmdVector[0] == "INVITE")
	// 	invite(fd, cmdVector);
}
