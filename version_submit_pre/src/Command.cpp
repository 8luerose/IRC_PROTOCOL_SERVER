#include "../includes/Command.hpp"

Command::Command(Server &server) : _server(server)
{
}

Command::~Command()
{
}

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

	//만약 "CAP LS"라는 문자열이 들어올 경우 아무것도 하지 않는 코드
	if (serverMsg.str().find("CAP LS") != std::string::npos)
		return ;
	
	// 석준 추가
	// \n이 없는경우 eof발생이라 판단
	if (serverMsg.str().find("\n") == std::string::npos)
	{
		clientList[fd].getEofBuffer() << serverMsg.str();
		clientList[fd].setEofFlag(true);
		return;
	}

	// 태현 추가
	std::string irssiTest = serverMsg.str();	// irssi 전용
	if (irssiTest.find("PASS") != std::string::npos &&	\
        irssiTest.find("NICK") != std::string::npos &&	\
        irssiTest.find("USER") != std::string::npos)
        irssiSignUp(fd, irssiTest);
	else
	{	// irssi가 아닌 경우
		std::cout << "#serverMsg" << serverMsg.str() << std::endl;
		    // eofFlag가 true인 경우 eofBuffer를 serverMsg의 앞쪽에 붙임
		if (clientList[fd].getEofFlag() == true)
		{
			std::stringstream temp;
			temp << clientList[fd].getEofBuffer().str() << serverMsg.str();
			serverMsg.str(temp.str());
			clientList[fd].setEofFlag(false);
			clientList[fd].getEofBuffer().str(""); // eofBuffer 초기화
		}
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
	}
	std::cout << "#printCmdVector" << std::endl;
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

void Command::irssiSignUp(int fd, std::string irssiFullCommand)
{
    std::stringstream ss(irssiFullCommand);
    std::string line;

    while (std::getline(ss, line, '\n'))	// 개행문자를 기준으로 명령어를 '반복해서' 나눔
	{	// 1차례: PASS 1234\n
		// 2차례: NICK kwon\n
		// 3차례: USER <username> <hostname> <servername> :<realname>\n
    	std::vector<std::string> cmdVector;
        std::stringstream tmp(line);		// 개행까지 받은 걸 임시로 저장 ex) PASS 1234\n
        std::string cmd;

		while (tmp >> cmd)						// stringstream을 이용해 공백을 기준으로 명령어를 나눔
		{
			if (cmdVector.empty())
				cmdVector.push_back(cmd); // 첫 번째 명령어 저장, 예: "USER"
			else if (cmdVector[0] == "USER")
			{
                std::string username, hostname, servername, realname;
				std::cout << "#stream!!" << std::endl;
				std::cout << "#tmp: " << tmp.str() << std::endl;
				std::cout << "#cmd: " << cmd << std::endl;
				std::stringstream userStream;
				size_t usernamePos = tmp.str().find("USER");
				std::string userStr = tmp.str().substr(usernamePos + 4);
				userStream << userStr;
				std::cout << "#userStream: " << userStream.str() << std::endl;
				userStream >> username >> hostname >> servername; // 다음 세 파라미터를 읽음
				getline(userStream, realname); // 실제 이름은 콜론을 포함하여 저장

				//realname 에 공백만 자르기 위해
				size_t pos = realname.find_first_not_of(" "); // 공백이 아닌 첫 문자의 위치
				realname = realname.substr(pos); // 공백이 아닌 첫 문자부터 끝까지의 문자열을 반환
                cmdVector.push_back(username);
				std::cout << "#username: " << username << std::endl;
                cmdVector.push_back(hostname);
				std::cout << "#hostname: " << hostname << std::endl;
                cmdVector.push_back(servername);
				std::cout << "#servername: " << servername << std::endl;
                cmdVector.push_back(realname); // 실제 이름은 콜론을 포함하여 저장
				std::cout << "#realname: " << realname << std::endl;
				std::cout << "#cmdVector[0] : " << cmdVector[0] << std::endl;
				std::cout << "#cmdVector[1] : " << cmdVector[1] << std::endl;
				std::cout << "#cmdVector[2] : " << cmdVector[2] << std::endl;
				std::cout << "#cmdVector[3] : " << cmdVector[3] << std::endl;
				std::cout << "#cmdVector[4] : " << cmdVector[4] << std::endl;
                break; // 더 이상의 파싱은 필요 없음
			}
			else
				cmdVector.push_back(cmd);
		}

        if (!cmdVector.empty())
		{	// ==== 대문자로 변환 ====
            std::string command = cmdVector[0];
            for (size_t i = 0; i < command.length(); i++)
                command[i] = std::toupper(command[i]);
			// ==== 대문자 변경 완료 ====

			// ex) cmdVector = {PASS, 1234}
            if (command == "PASS")
                pass(fd, cmdVector);
            else if (command == "NICK")
                nick(fd, cmdVector);
            else if (command == "USER")
                user(fd, cmdVector);
        }
    }
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator iter = clientList.find(fd);
		// 첫 실행 시, 인사말 출력
	iter = clientList.find(fd);
	if (iter != clientList.end())
	{
		if (iter->second.getIsRegist())
			iter->second.appendReciveBuf(":IRC 001 " + iter->second.getNickname() + " :Welcome to the Interget Relay Network " + iter->second.getNickname() + "!" + iter->second.getUsername() + "@" + iter->second.getHostname() + "\r\n");
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
		else if (cmdVector[0] == "WHOIS")
			;
		else if (cmdVector[0] == "WHO")
			;
		else
        {
            // 등록되어 있지 않은 명령어의 경우 에러처리
            std::map<int, Client>& clientList = _server.getClientList();
            std::map<int, Client>::iterator iter = clientList.find(fd);
            if (iter != clientList.end())
                ERROR_unknowncommand_421(iter->second, cmdVector[0]);
        }
	}
}
