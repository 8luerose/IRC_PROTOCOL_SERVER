#include "../includes/Command.hpp"

void Command::pass(int fd, std::vector<std::string> command_vec)
{	/* PASS <password> */
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator iter;
	
	iter = clientList.find(fd);
	if (iter->second.getRegiPass() == true)
	{
		ERROR_alreadyregistred_462(iter->second);
		return;
	}

	if (command_vec.size() < 2)
	{
		ERROR_needmoreparams_461(iter->second);
		iter->second.appendReciveBuf("/PASS <password>\r\n");
		return;
	}

	std::string userPassword = _server.getPassword();	// "/ircserv 6667 1234" 중에서 -> "1234"가 '서버 비밀번호'
	if (command_vec[1] != userPassword)
	{
		ERROR_passwdmismatch_464(iter->second);
		send(fd, iter->second.getReciveBuf().c_str(), iter->second.getReciveBuf().length(), 0);
		iter->second.resetClient();
		clientList.erase(fd);
		close(fd);
		return;
	}

	iter->second.setRegiPass(true);	// Client가 서버 Password 인증 완료한 상태
}