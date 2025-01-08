#include "../includes/Command.hpp"

void Command::mode(int fd, std::vector<std::string> cmdVector)
{	// "/MODE <channel>	 <+/- i, t, k, l, o>   <k,l,o 에 해당하는 values 순서지켜서!!>"
	// ex) "MODE #channel +itkl 1234 2"
	std::cout << "#MODE" << std::endl;
	
	// ** 채널모드 '조회' 출력 결과 <-> 채널모드 '설정' 출력 결과 = 다름!!! **
	// 채널 조회란? 채널 모드를 조회하려면 단순히 MODE 명령어 뒤에 채널 이름만 입력
		// MODE #channel
		// 324 #example +kt secret
	// 채널 설정이란? 채널 모드를 설정할 때는 MODE 명령어 뒤에 채널 이름과 설정하고자 하는 모드를 지정
		// MODE #channel +o nickname
		// :nickname!user@host MODE #channel +o othernickname
	
	Client &client = _server.getClientList().find(fd)->second;
	if (cmdVector.size() < 2)
	{
		ERROR_needmoreparams_461(client);
		return;
	}
	std::string argvChannelName = cmdVector[1];

    if (client.getNickname() == argvChannelName)
        return;

	Channel *channel = _server.findChannel(argvChannelName);

	if (channel == NULL)
	{	// /Mode 인자인 '#channel'을 서버에 저장된 채널 리스트에서 못 찾았을 때
		ERROR_nosuchchannel_403(client, argvChannelName);	// (Client &, std::string)
		return;
	}

	// 채널 조회	ex) MODE #channel
	if ((channel != NULL) && (cmdVector.size() == 2))
	{	// /Mode 인자인 '#channel'을 서버에 저장된 채널 리스트에서 찾았고!
		// and, "/Mode #channel" 입력됐을 때
        std::string modeParams = "";
		std::string limitValue = "";
		if (channel->getMode().find('t') != std::string::npos)
		{
			modeParams += "";
		}
        if (channel->getMode().find('k') != std::string::npos)
		{
			if (channel->diffOperator(fd))	// 오퍼레이터일 때
				modeParams += channel->getKey() + " ";
			else							// 오퍼레이터가 아닐 때 -> 키 조회 불가
				modeParams += "<key> ";
		}
        if (channel->getMode().find('l') != std::string::npos)
		{
			std::ostringstream oss;	// ostringstream은 output stream의 약자로, 문자열을 출력하기 위한 스트림
            oss << channel->getLimit();
			limitValue += oss.str();
		}			
		if (modeParams.empty() && limitValue.empty())	// 토픽, 키, limit '없을 때'
			client.appendReciveBuf(":" + std::string(PREFIX_SERVERNAME) + " 324 " + client.getNickname() + " " + argvChannelName + " :+" + channel->getMode() + "\r\n");
		else											// 토픽, 키, limit '있을 떄'
			client.appendReciveBuf(":" + std::string(PREFIX_SERVERNAME) + " 324 " + client.getNickname() + " " + argvChannelName + " +" + channel->getMode() + " " + modeParams + ":" + limitValue + "\r\n");
        return;
	}

	std::string modeArgv = cmdVector[2];	// ex) <+/- i, t, k, l, o>
	if (modeArgv.length() == 1)				// MODE #gen b 이렇게만 들어왔을 때 종료
		return;


	if (channel != NULL && !channel->diffOperator(fd))
	{	// 채널이 존재하고, 채널 오퍼레이터가 아닐 때
		ERROR_chanoprivsneeded_482(client, argvChannelName);	// (Client &, std::string)
		return;
	}

	std::string message = "";
	std::string plus_message = "";
    std::vector<std::string> modeValueList;	// <t,k,l,o 에 해당하는 values>
    unsigned int modeValueCnt = 3;
	// "/Mode #channel +i <modeValueList>" 이므로 Value는 4부터 시작이므로, 3으로 초기화 해서, Value 만나면 그때서야 확신 갖고 ++증가
	char sign = '\0';

	size_t idx = 0;
	while (idx < modeArgv.length() && (modeArgv[idx] == '+' || modeArgv[idx] == '-'))
	{
		sign = modeArgv[idx];
		idx++;
	}
	if (modeArgv.length() == 1)							// +혹은 -만 입력됐을 때 종료
		return;

	// 채널모드 설정 ex) "+o nickname"
	for (size_t i = idx; i < modeArgv.length(); i++)
    {
        if (modeArgv[i] == '+' || modeArgv[i] == '-')
        {
            sign = modeArgv[i];
            continue;
        }

        bool isSetMode = false;							// 모드 '정상' 설정됐는지 여부
        if (modeArgv[i] == 'i')
        {
            if (sign == '+' && channel->diffMode('i'))	// +들어왔는데 이미 채널에 i 모드가 설정되어 있을 때
			{
				std::cout << "+인데 i모드 설정 되어 있음" << std::endl;
				continue;
			}
            if (sign == '-' && !channel->diffMode('i'))	// -들어왔는데 채널에 i 모드가 설정되어 있지 않을 때
                continue;
            channel->setMode('i', sign, fd);
            isSetMode = true;
        }
        else if (modeArgv[i] == 't')					// 설정 허용만, /TOPIC 해야 비로소 토픽 설정됨
        {
            if (sign == '+' && channel->diffMode('t'))
                continue;
            if (sign == '-' && !channel->diffMode('t'))
                continue;
            channel->setMode('t', sign, fd);
            isSetMode = true;
        }
        else if (modeArgv[i] == 'k')
        {
            if (sign == '-' && channel->diffMode('k') == false)
			{
				std::cout << "#-인데 k모드 설정 안되어 있음" << std::endl;
				continue;
			}
            if (cmdVector.size() > modeValueCnt)
            {
                if (sign == '+')
                {
                    channel->setKey(cmdVector[modeValueCnt]);
					channel->setMode('k', sign, fd);
					isSetMode = true;
                	modeValueList.push_back(cmdVector[modeValueCnt]);	// <t,k,l,o 에 해당하는 values> 저장
                	modeValueCnt++;
                }
            }
			if (sign == '-')
			{
				channel->setKey("");
				channel->setMode('k', sign, fd);
				isSetMode = true;
			}
        }
        else if (modeArgv[i] == 'l')
        {
            if (sign == '-' && channel->diffMode('l') == false)			//해제 부호인데, 설정 안되어 있으면 해제ㄴㄴ 무시
			{
				std::cout << "#-인데 l모드 설정 안되어 있음" << std::endl;
				continue;
			}
            if (cmdVector.size() > modeValueCnt)
            {
                std::string limitValue = cmdVector[modeValueCnt].c_str();	//  <t,k,l,o 에 해당하는 values> 중 ValueCnt ++증가된 상태
                bool isDigit = true;
                for (size_t j = 0; j < limitValue.length(); ++j)
                {
                    if (!isdigit(limitValue[j]))
                    {
                        isDigit = false;
                        break;
                    }
                }
                if (isDigit == false)
                {
                    modeValueCnt++;
                    continue;
                }
                int limit = atoi(limitValue.c_str());
                if (limit < 0)
                {
                    modeValueCnt++;
                    continue;
                }
                if (sign == '+')
                {
                    channel->setLimit(limit);
					channel->setMode('l', sign, fd);
					isSetMode = true;
                	modeValueList.push_back(cmdVector[modeValueCnt]);
                	modeValueCnt++;
                }
            }
			if (sign == '-')
			{
				channel->setLimit(0);
				channel->setMode('l', sign, fd);
				isSetMode = true;
			}
        }
        else if (modeArgv[i] == 'o')
        {
            if (cmdVector.size() <= modeValueCnt)	
            {
				continue;				// /Mode #channel +o 입력했는데, nickname이 없을 때 +o 무시
            }
            std::map<int, Client>::iterator target = _server.findClient(cmdVector[modeValueCnt]);	// +o 뒤에 오는 "nickname" 찾는다
            if (target == _server.getClientList().end())	// nickname 못 찾았을 때 에러
            {
				ERROR_nosuchnick_401(client, cmdVector[modeValueCnt]);
                return;
            }
            else	// nickname 찾았을 때
            {
                if (client.getNickname() == target->second.getNickname())	// 자기 자신에게는 +o 불가
                {
                    return;
                }
				if (!(channel->diffClientInChannel(target->second.getClientFd())))	// 채널에 없는 클라이언트에게 +o 불가
                {
					ERROR_usernotinchannel_441(client, cmdVector[modeValueCnt], cmdVector[1]);
                    return;
                }
                else if (sign == '+')
                {
					std::cout << "#타켓 fd : " << target->second.getClientFd() << std::endl;
					plus_message = target->second.getNickname();
					std::cout << "#타겟 닉네임 :[nickname] " << target->second.getNickname() << std::endl;
					channel->setMode('o', sign, target->second.getClientFd());
					if (channel->diffOperator(target->second.getClientFd()))
						std::cout << "#오퍼레이터 추가 완료" << std::endl;
					else
						std::cout << "#오퍼레이터 추가 실패" << std::endl;
                    isSetMode = true;
                    modeValueCnt++;
                }
                else if (sign == '-')
                {
					if (!channel->diffOperator(fd))
					{
						ERROR_chanoprivsneeded_482(client, argvChannelName);
						return;
					}
					std::cout << "#타켓 fd : " << target->second.getClientFd() << std::endl;
					plus_message = target->second.getNickname();
					std::cout << "#타겟 닉네임 :[nickname] " << target->second.getNickname() << std::endl;
                    // channel->removeOperatorFd(target->second.getClientFd()); -> setMode(, ,타겟 fd)
					channel->setMode('o', sign, target->second.getClientFd());
					if (channel->diffOperator(target->second.getClientFd()))
						std::cout << "#타겟 오퍼레이터 삭제 안 됨" << std::endl;
					else
						std::cout << "#타겟 오퍼레이터 삭제 됨" << std::endl;
					if (channel->diffOperator(fd))
						std::cout << "#방장 권한 유지" << std::endl;
					else
						std::cout << "#방장 권한 삭제됨" << std::endl;
                    isSetMode = true;
                    modeValueCnt++;
                }
            }
        }
        else		// <+/- i, t, k, l, o> 중에 없으면 에러
        {
			ERROR_unknownmode_472(client, modeArgv[i]);
            continue;
        }
        if (isSetMode)	// 모드 '정상' 설정하고 끝났다면
        {
            if (message.empty())
                message += sign;	// ex) '+' 차곡차곡
            message += modeArgv[i];	// ex) +'itkl' 차곡차곡
        }
    }
	
	// 방장이 아닐 때 추가 인자 붙여주지 않는다 && 방장일 때 +kl 1234 2 추가 인자 붙여준다
	if (channel->diffOperator(fd) == true)
	{
		for (size_t i = 0; i < modeValueList.size(); ++i)
		{
			if (!modeValueList[i].empty())			// 비어있지 않을 때
				message += " " + modeValueList[i];	// ex) "+l -k 2 key"
		}
	}
	if (message.empty())
		return;

	plus_messageAllChannel(fd, cmdVector[1], "MODE", message, plus_message);	// #채널에 MODE와 메세지 전달
}
