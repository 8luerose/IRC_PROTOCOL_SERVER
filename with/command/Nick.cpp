#include "../includes/Command.hpp"


void Command::nick(int fd, std::vector<std::string> cmdVector)
{
	// "/NICK <nickname>"
	std::map<int, Client>& clientList = _server.getClients();
	std::map<int, Client>::iterator iter;

	iter = clientList.find(fd);
	if (!(iter->second.getRegiPass()))
	{
		ERROR_notregistered_451(iter->second);
		send(fd, iter->second.getReciveBuf().c_str(), iter->second.getReciveBuf().length(), 0);
		iter->second.resetClient();	// 그 클라이언트 상태 초기화
		clientList.erase(fd);		// 클라이언트 리스트에서 삭제
		close(fd);					// 그 클라이언트 소켓 닫기
		return;
	}
}