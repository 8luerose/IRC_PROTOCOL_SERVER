#include "../includes/Command.hpp"

void Command::ping(int fd, std::vector<std::string> cmdVector)
{
	// PING <server1> [<server2>]
	// PING 명령어는 서버간의 연결을 확인하기 위해 사용됨 (server2는 생략 가능)
		// 적절한 PONG 메시지로 응답하여 아직 연결되어 있고 살아있음을 표시해야 함
		// 정기적으로 보내야 하고, 연결이 설정된 시간 내에 PING 명령에 응답하지 않으면 시간 내에 응답하지 않으면 해당 연결이 닫힘
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator clientIt = clientList.find(fd);

	if (clientIt == clientList.end())	// fd 클라이언트가 클라이언트 리스트에 존재하지 않을 경우
		return;

	if (cmdVector.size() < 2)	// server1이 없을 경우
	{
		ERROR_needmoreparams_461(clientIt->second);
		clientIt->second.appendReciveBuf("/PING <token>\r\n");
		return;
	}
	clientIt->second.appendReciveBuf(":" + std::string(PREFIX_SERVERNAME) + " PONG " + std::string(PREFIX_SERVERNAME) + " :" + cmdVector[1] + "\r\n");
}