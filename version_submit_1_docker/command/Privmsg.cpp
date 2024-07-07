#include "../includes/Command.hpp"

void Command::privmsg(int fd, std::vector<std::string> cmdVector)
{
	Client &client = _server.getClientList().find(fd)->second;
	if (cmdVector.size() < 3) // 명령어에 필요한 인자가 부족한 경우 :
	{
		ERROR_needmoreparams_461(client);
		return;
	}
	std::vector<std::string> msgArgv1 = split(cmdVector[1], ',');
	std::vector<std::string>::iterator msgArgv1Iter = msgArgv1.begin();
	for (; msgArgv1Iter != msgArgv1.end(); msgArgv1Iter++) // 채널 목록을 순회
	{
		if ((*msgArgv1Iter)[0] == '#' || (*msgArgv1Iter)[0] == '&') // 채널인 경우 :
		{
			std::vector<std::string>::iterator channel_iter = client.findChannel(*msgArgv1Iter);
			if (channel_iter == client.getChannelList().end())
			{
				ERROR_cantsendtochan_404(client, *msgArgv1Iter);
				return;
			}
			else
			{
				messageAllChannel(fd, *msgArgv1Iter, "PRIVMSG", channelMessage(2, cmdVector));
			}

		}
		else
		{
			std::map<int, Client>::iterator client_iter = _server.findClient(*msgArgv1Iter);
			if (client_iter != _server.getClientList().end())
			{
				std::string message = channelMessage(2, cmdVector);
				client_iter->second.appendReciveBuf(makeMsgForm(fd, cmdVector[0]) + " PRIVMSG " + client.getNickname() + " :" + message + "\r\n");
			}
			else
			{
				ERROR_nosuchnick_401(client_iter->second, *msgArgv1Iter);
				continue;
			}
		}
	}
}
