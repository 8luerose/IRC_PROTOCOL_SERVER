#include "../includes/Command.hpp"

void Command::user(int fd, std::vector<std::string> cmdVector)
{	//	"/USER <username> <hostname> <servername> <:realname>"
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator iter;
	std::string realnameArgv;

	iter = clientList.find(fd);
	if ((iter->second.getRegiUser()) == true)	// 이미 등록된 유저인 경우
	{
		ERROR_alreadyregistred_462(iter->second);
		return;
	}

	if ((iter->second.getRegiPass()) == false)	// Client가 pass 인증이 안되어있으면
	{
		ERROR_notregistered_451(iter->second);
		send(fd, iter->second.getReciveBuf().c_str(), iter->second.getReciveBuf().length(), 0);
		iter->second.resetClient();	// 그 클라이언트 상태 초기화
		clientList.erase(fd);		// 클라이언트 리스트에서 삭제
		close(fd);					// 그 클라이언트 소켓 닫기
		return;
	}

	if (cmdVector.size() < 5)		// USER <username> <hostname> <servername> <:realname> 5개 이상이 아닌 경우
	{
		std::cout << "#// USER <username> <hostname> <servername> <:realname> 5개 이상이 아닌 경우" << std::endl;
		std::cout << "#cmdVector.size() : " << cmdVector.size() << std::endl;
		// std::cout << "#cmdVector[0] : " << cmdVector[0] << std::endl;
		// std::cout << "#cmdVector[1] : " << cmdVector[1] << std::endl;
		// std::cout << "#cmdVector[2] : " << cmdVector[2] << std::endl;
		// std::cout << "#cmdVector[3] : " << cmdVector[3] << std::endl;
		// std::cout << "#cmdVector[4] : " << cmdVector[4] << std::endl;
		ERROR_needmoreparams_461(iter->second);
		iter->second.appendReciveBuf("/USER <username> <hostname> <servername> <:realname>\r\n");
		return;
	}

	if (cmdVector.size() > 5 && cmdVector[4][0] != ':')	// realname이 :로 시작하지 않는 경우
	{
		std::cout << "#// realname이 :로 시작하지 않는 경우" << std::endl;
		std::cout << "#cmdVector.size() : " << cmdVector.size() << std::endl;
		// std::cout << "#cmdVector[0] : " << cmdVector[0] << std::endl;
		// std::cout << "#cmdVector[1] : " << cmdVector[1] << std::endl;
		// std::cout << "#cmdVector[2] : " << cmdVector[2] << std::endl;
		// std::cout << "#cmdVector[3] : " << cmdVector[3] << std::endl;
		// std::cout << "#cmdVector[4] : " << cmdVector[4] << std::endl;
		ERROR_needmoreparams_461(iter->second);
		iter->second.appendReciveBuf("/USER <username> <hostname> <servername> <:realname>\r\n");
		return;
	}

	for (size_t idx = 4; idx < cmdVector.size(); idx++)
	{
		realnameArgv += cmdVector[idx];
		if (idx != cmdVector.size() - 1)	// 마지막에는 공백 추가 안함 ex) last' '. (X)
			realnameArgv += " ";			// "KIM SU HYUN" 완성
	}

	iter->second.setUserinfo(cmdVector[1], cmdVector[2], cmdVector[3], realnameArgv);
	iter->second.setRegiUser(true);
	std::cout << "#USER 완료" << std::endl;
}

