#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::topic(int fd, std::vector<std::string> cmdVector)
{	// 양식: TOPIC <channel> <topic>
	// ex) TOPIC #test :another topic

	// ==== 인자 유효성 검사 ====
	if (cmdVector.size() < 2)												// 곧 cmdVector[1] 사용할거라, cmdVector[0] Topic만 있으면 안됨
	{
		ERROR_needmoreparams_461(_server.getClientList().find(fd)->second);
		// ERR_NEEDMOREPARAMS "Not enough parameters\r\n"
		return ;
	}
	// ==== 통과 ====
	Channel *channel = _server.findChannel(cmdVector[1]);					// <channel>을 찾는다

	// ==== 채널이 없는 경우 ====
	if (channel == NULL)													// 만약 채널이 없다면
	{
		ERROR_nosuchchannel_403(_server.getClientList().find(fd)->second, cmdVector[1]);
		// ERR_NOSUCHCHANNEL "No such channel\r\n"
		return ;
	}
	// ==== 통과 ====

	std::vector<int> clientFdList = channel->getFdListClient();				// 채널에 있는 클라이언트 목록(참여자들) 가져옮
	std::vector<int>::iterator clientIter;

	// ==== 클라이언트가 채널에 있는지 확인 ====
	clientIter = std::find(clientFdList.begin(), clientFdList.end(), fd);	// 채널's 클라이언트 목록에 -> fd가 있는지 확인
	if (clientIter == clientFdList.end())									// 클라이언트 목록에 없으면
	{
		ERROR_notonchannel_442(_server.getClientList().find(fd)->second, cmdVector[1]);
		// ERR_NOTONCHANNEL "You're not on that channel\r\n"				// 님네 채널 아님 물러가시오
		return ;
	}
	// ==== 통과 ====

	// ** 본격적으로 Topic 변경 **
	// Topic 변경 케이스 2가지
	// (1) 모드 +t인 경우: 채널 오퍼레이터만 변경 가능
	// (2) 모드 +t가 아닌 경우: 누구나 변경 가능


	// // ==== (1) 모드 +t인 경우: 채널 오퍼레이터만 변경 가능 ====
	// if (channel->diffMode('t'))												// 모드 +t인 경우 -> "님 방장임?" -> 아니라면, 밑에 로직으로 넘어가지말고 걍 나가셈(return)
	// {
	// 	if (channel->diffOperator(fd) == false)								// 채널 오퍼레이터가 아니면
	// 	{
	// 		ERROR_chanoprivsneeded_482(_server.getClientList().find(fd)->second, cmdVector[1]);
	// 		// ERR_CHANOPRIVSNEEDED "You're not channel operator\r\n"		// 님 방장 아님
	// 		return ;
	// 	}
	// }
	// // ==== 통과 ====

	// ==== (2) 모드 +t가 아닌 경우: 누구나 변경 가능 ====
	if (cmdVector.size() == 2)
	{	// TOPIC <channel> 만 입력된 경우 -> Topic 조회

		// ==== 채널 유효성 검사 ====
		if (channel == NULL)												// 만약 채널이 없으면 <-> 인자 부족 461 에러랑은 다름
		{
			ERROR_nosuchchannel_403(_server.getClientList().find(fd)->second, cmdVector[1]);
			// ERR_NOSUCHCHANNEL "No such channel\r\n"
			return ;
		}
		// ==== 통과 ====

		if (channel->getTopic().empty())
		{	// 기존 Topic 없는 경우
			// topic #gen
			// :irc.local 331 a #gen :No topic is set.
			_server.getClientList().find(fd)->second.appendReciveBuf(std::string(PREFIX_SERVERNAME) + " 331 " + _server.getClientList().find(fd)->second.getNickname() + " " + cmdVector[1] + " :" + RPL_NOTOPIC + "\r\n");

			// RPL_NOTOPIC "No topic is set\r\n"
			return ;
		}
		else
		{
			_server.getClientList().find(fd)->second.appendReciveBuf(std::string(PREFIX_SERVERNAME) + " 332 " + _server.getClientList().find(fd)->second.getNickname() + " " + cmdVector[1] + " :" + channel->getTopic() + "\r\n");
		}	
	}
	else
	{	// TOPIC <channel> <topic> 입력된 경우 -> Topic 변경
		// ==== (1) 모드 +t인 경우: 채널 오퍼레이터만 변경 가능 ====
		if (channel->diffMode('t'))												// 모드 +t인 경우 -> "님 방장임?" -> 아니라면, 밑에 로직으로 넘어가지말고 걍 나가셈(return)
		{
			if (channel->diffOperator(fd) == false)								// 채널 오퍼레이터가 아니면
			{
				ERROR_chanoprivsneeded_482(_server.getClientList().find(fd)->second, cmdVector[1]);
				// ERR_CHANOPRIVSNEEDED "You're not channel operator\r\n"		// 님 방장 아님
				return ;
			}
		}
		// ==== 통과 ====

		if (cmdVector[2] == ":")											// 만약 ':'만 들어오면 빈 주제 -> 원래는, ":another topic" 이렇게 ':' 이거 입력하고 topic 들어오는게 정석
			channel->setTopic("");											// 빈 주제니까 빈 문자열로 설정
		else
		{	// ':' 이후의 문자열을 모두 합쳐서 Topic으로 설정
			std::string str = cmdVector[2];									// ":another" (cmdVector[2]) "topic" (cmdVector[3]) ..
			for (size_t i = 3; i < cmdVector.size(); i++)
				str += " " + cmdVector[i];									//  -> ":another topic"
			if (str[0] == ':')												// ':'로 시작하면 ':' 제거 -> messageAllChannel에서 이미 씀 -> " :" + message + "\r\n")
				str = str.substr(1);										// ':' 제거
			channel->setTopic(str);											// 채널 토픽(std::string)으로 저장
		}
		messageAllChannel(fd, cmdVector[1], "TOPIC", channel->getTopic());	// 채널에 있는 모든 클라이언트에게 TOPIC 메시지 전송
		// == " : name ! user @ fd TOPIC #channel :another topic \r\n"
	}
}