#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::invite(int fd, std::vector<std::string> cmdVector)
{	// *유의* 까먹지 말자 여기서 fd는 명령어를 '사용'하려고 하는 클라이언트의 fd!!

	// INVITE <nickname> <channel> -> MODE 에서 +i 설정 시 초대 유효 (설정 안 되어 있어도 초대 기능 작동은 함)
	Client &client = _server.getClientList().find(fd)->second;	// map<int, Client>에서 (fd)를 키로 가지는 Client value를 찾아 반환

	if (cmdVector.size() < 3)	// "INVITE, <nickname>, <channel>" 안 들어올 경우
	{
		ERROR_needmoreparams_461(client);
		return;
	}

	std::map<int, Client>::iterator target = _server.findClient(cmdVector[1]);	// map<int, Client>에서 (cmdVector[1] == nickname)를 키로 가지는 Client value를 찾아 반환
	Channel *channel = _server.findChannel(cmdVector[2]);						// cmdVector[2] == #channel
	// 대상이 서버에 접속해있는지, 대상이 <channel>에 있는지
	
	// ===== 인가 확인 작업 =====
	if (target == _server.getClientList().end())								// findClient()에서 end반환 했을 시 -> 클라이언트 리스트에 없다는 뜻 -> 못 찾았으니 에러
	{
		ERROR_nosuchnick_401(client, cmdVector[1]);
		return;
	}
	if (channel == NULL)														// findChannel()에서 NULL 반환 했을 시 -> 채널 리스트에 없다는 뜻 -> 못 찾았으니 에러
	{
		ERROR_nosuchchannel_403(client, cmdVector[2]);
		return;
	}
	if (channel->diffClientInChannel(client.getClientFd()) == false)			// 채널리스트에서, '명령어를 사용'하려고 하는 닉네임이 있는지 '비교'해봤는데, false 일 경우
	{
		ERROR_notonchannel_442(client, cmdVector[2]);
		return;
	}
	if (channel->diffOperator(client.getClientFd()) == false)					// "야 너 방장 맞음??" -> 채널리스트에서, '명령어를 사용'하려고 하는 닉네임이 '오퍼레이터'인지 '비교'해봤는데, false 일 경우
	{
		ERROR_chanoprivsneeded_482(client, cmdVector[2]);
		return;
	}
	if (channel->diffClientInChannel(target->second.getClientFd()) == true)		// "야 걔 방에 있음??" -> 채널리스트에서, '초대받을 사람'이 있는지 '비교'해봤는데, true 일 경우
	{
		ERROR_useronchannel_443(client, cmdVector[1], cmdVector[2]);
		return;
	}
	// ==== 통과 =====

	target->second.appendReciveBuf(":" + client.getNickname() + " INVITE " + target->second.getNickname() + " " + cmdVector[2] + "\r\n");
	// "명령어 사용하는 클라이언트 닉네임: '명령어 사용자 닉네임' 'INVITE' '초대받을 사람 닉네임' '#채널'"
	// == "누가 너 초대했는디?"

	client.appendReciveBuf("341 " + client.getNickname() + " " + cmdVector[1] + " " + cmdVector[2] + " :" + RPL_INVITING);
	// "명령어 사용하는 클라이언트 닉네임 '341' '명령어 사용자 닉네임' '#채널' 'RPL_INVITING'"
	// == "야 너 누구 초대했더라 명령어 먹힘ㅇㅇ"

	channel->appendFdListInvite(target->second.getClientFd());
	// 채널리스트에서, '초대받을 사람'의 fd를 초대 리스트에 추가
}