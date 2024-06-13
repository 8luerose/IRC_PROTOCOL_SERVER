#include "../includes/Command.hpp"

void Command::mode(int fd, std::vector<std::string> cmdVector)
{	// "/MODE <channel>	 <+/- i, t, k, l, o>   <t,k,l,o 에 해당하는 values 순서지켜서!!>"
	
	// ** 채널모드 '조회' 출력 결과 <-> 채널모드 '설정' 출력 결과 = 다름!!! **
	// 채널 조회란? 채널 모드를 조회하려면 단순히 MODE 명령어 뒤에 채널 이름만 입력
		// MODE #channel
		// 324 #example +kt secret
	// 채널 설정이란? 채널 모드를 설정할 때는 MODE 명령어 뒤에 채널 이름과 설정하고자 하는 모드를 지정
		// MODE #channel +o nickname
		// :nickname!user@host MODE #channel +o othernickname
	
	// 설정은, 채널 조회처럼 "324  "<channel> <mode> <mode params>" '324' 응답코드 없음!

	Client &client = _server.getClientList().find(fd)->second;
	if (cmdVector.size() < 2)
	{
		ERROR_needmoreparams_461(client);
		return;
	}
	std::string argvChannelName = cmdVector[1];	// #channel of /Mode 
	Channel *channel = _server.findChannel(argvChannelName);
	// findChannel 반환값 : (Channel*) or (NULL)
	
	// 에러
	if ((channel == NULL) && (client.getNickname() != argvChannelName))
	{	// /Mode 인자인 '#channel'을 서버에 저장된 채널 리스트에서 못 찾았고
		// and 애초에 (fd != argvChannelName) 일 때
		ERROR_nosuchchannel_403(client, argvChannelName);	// (Client &, std::string)
		return;
	}

	// 채널 조회
	if ((channel != NULL) && (cmdVector.size() == 2))
	{	// /Mode 인자인 '#channel'을 서버에 저장된 채널 리스트에서 찾았고!
		// and, "/Mode #channel" 입력됐을 때
        std::string modeParams = "";

        if (channel->getMode().find('k') != std::string::npos)
            modeParams += " " + channel->getKey();
        if (channel->getMode().find('l') != std::string::npos)
		{
			std::ostringstream oss;	// ostringstream은 output stream의 약자로, 문자열을 출력하기 위한 스트림
            oss << channel->getLimit();
            modeParams += " " + oss.str();
		}			
		client.appendReciveBuf(argvChannelName + " +" + channel->getMode() + modeParams + "\r\n");
        return;
		// == "<channel> <mode> <mode params>"
		// == #example +kt secret
		// <mode params> == <+/- i, t, k, l, o> mode 뒤에 오는 "<t,k,l 에 해당하는 values>"
	}

	if (channel != NULL && !channel->diffOperator(fd))
	{	// 채널이 존재하고, 채널 오퍼레이터가 아닐 때
		ERROR_chanoprivsneeded_482(client, argvChannelName);	// (Client &, std::string)
		return;
	}

	std::string modeArgv = cmdVector[2];
	std::string message = "";
    std::vector<std::string> modeValueList;
    unsigned int modeValueCnt = 3;
	// "/Mode #channel +i <modeValueList>" 이므로 Value는 4부터 시작인데, 3으로 초기화 해서, Value 만나면 ++증가
	char sign = '\0';

	// 수정 중
	// for (size_t i = 0; i < mode.length(); i++)
    // {
    //     if (mode[i] == '+' || mode[i] == '-')
    //     {
    //         sign = mode[i];
    //         continue;
    //     }
    //     bool isSetMode = false;
    //     if (mode[i] == 'i')
    //     {
    //         if (sign == '+' && channel->checkMode(INVITE))
    //             continue;
    //         if (sign == '-' && !channel->checkMode(INVITE))
    //             continue;
    //         channel->setMode(INVITE, sign);
    //         isSetMode = true;
    //     }
    //     else if (mode[i] == 't')
    //     {
    //         if (sign == '+' && channel->checkMode(TOPIC))
    //             continue;
    //         if (sign == '-' && !channel->checkMode(TOPIC))
    //             continue;
    //         channel->setMode(TOPIC, sign);
    //         isSetMode = true;
    //     }
    //     else if (mode[i] == 'k')
    //     {
    //         if (sign == '-' && !channel->checkMode(KEY))
    //             continue;
    //         if (cmdVector.size() > modeArgIndex)
    //         {
    //             channel->setMode(KEY, sign);
    //             if (sign == '+')
    //             {
    //                 channel->setKey(cmdVector[modeArgIndex]);
    //             }
    //             else if (sign == '-')
    //             {
    //                 channel->setKey("");
    //             }
    //             isSetMode = true;
    //             modeArgList.push_back(cmdVector[modeArgIndex]);
    //             modeArgIndex++;
    //         }
    //         else
    //         {
    //             err_needmoreparams_461(client);
    //             return;
    //         }
    //     }
    //     else if (mode[i] == 'l')
    //     {
    //         if (sign == '-' && !channel->checkMode(LIMIT))
    //             continue;
    //         if (cmdVector.size() > modeArgIndex)
    //         {
    //             std::string limit_s = cmdVector[modeArgIndex];
    //             bool isDigit = true;
    //             for (size_t j = 0; j < limit_s.length(); ++j)
    //             {
    //                 if (!isdigit(limit_s[j]))
    //                 {
    //                     isDigit = false;
    //                     break;
    //                 }
    //             }
    //             if (!isDigit)
    //             {
    //                 modeArgIndex++;
    //                 continue;
    //             }
    //             int limit = atoi(limit_s.c_str());
    //             if (limit < 0)
    //             {
    //                 modeArgIndex++;
    //                 continue;
    //             }
    //             channel->setMode(LIMIT, sign);
    //             if (sign == '+')
    //             {
    //                 channel->setLimit(limit);
    //             }
    //             isSetMode = true;
    //             modeArgList.push_back(cmdVector[modeArgIndex]);
    //             modeArgIndex++;
    //         }
    //         else
    //         {
    //             err_needmoreparams_461(client);
    //             return;
    //         }
    //     }
    //     else if (mode[i] == 'o')
    //     {
    //         if (cmdVector.size() <= modeArgIndex)
    //         {
    //             err_needmoreparams_461(client);
    //             return;
    //         }
    //         std::map<int, Client>::iterator target = _server.findClient(cmdVector[modeArgIndex]);
    //         if (target == _server.getClients().end())
    //         {
    //             err_nosuchnick_401(client, cmdVector[modeArgIndex]);
    //             return;
    //         }
    //         else
    //         {
    //             if (client.getNickname() == target->second.getNickname())
    //             {
    //                 return;
    //             }
    //             if (!channel->checkClientInChannel(target->second.getClientFd()))
    //             {
    //                 err_usernotinchannel_441(client, cmdVector[modeArgIndex], cmdVector[1]);
    //                 return;
    //             }
    //             else if (sign == '+')
    //             {
    //                 channel->addOperatorFd(target->second.getClientFd());
    //                 isSetMode = true;
    //                 modeArgList.push_back(cmdVector[modeArgIndex]);
    //                 modeArgIndex++;
    //             }
    //             else if (sign == '-')
    //             {
    //                 channel->removeOperatorFd(target->second.getClientFd());
    //                 isSetMode = true;
    //                 modeArgList.push_back(cmdVector[modeArgIndex]);
    //                 modeArgIndex++;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         err_unknownmode_472(client, mode[i]);
    //         continue;
    //     }
    //     if (isSetMode)
    //     {
    //         if (msg.empty())
    //             msg += sign;
    //         msg += mode[i];
    //     }
    // }
    // for (size_t i = 0; i < modeArgList.size(); ++i)
    // {
    //     msg += " " + modeArgList[i];
    // }
    // if (msg.empty())
    //     return;
    // msgToAllChannel(fd, cmdVector[1], "MODE", msg);


}